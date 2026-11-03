/*
 * A007114  Expansion of e.g.f. (1 - x)^x.
 * https://oeis.org/A007114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   e.g.f. (1-x)^x = exp(x*log(1-x)), so a(n) = -Sum_{j=2..n}
 *            j*(j-2)!*binomial(n-1,j-1)*a(n-j), a(0) = 1 (equivalently a(n) =
 *            Sum_{k=0..n} (-1)^k * A008275(k,n-k) n!/k!)
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 15, i64 n <= 23, i128 n <= 36,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 200 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static seq_t from_i128(__int128 v)
{
    if (v >= -(__int128)4000000000000000000LL && v <= (__int128)4000000000000000000LL)
        return (seq_t)(long long)v;
    {
        __int128 q = v / ((__int128)1 << 32);
        long long rem = (long long)(v - q * ((__int128)1 << 32));
        return from_i128(q) * (seq_t)4294967296LL + (seq_t)rem;
    }
}

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static seq_t *memo = NULL;
static idx_t memo_len = 0;

static seq_t a_fixed(idx_t n) {
    /* All arithmetic modulo 2^128 (no division); the true value fits,
       so the low bits give it exactly. */
    unsigned __int128 *A = (unsigned __int128 *)malloc((n + 1) * sizeof(unsigned __int128));
    unsigned __int128 *C = (unsigned __int128 *)malloc((n + 1) * sizeof(unsigned __int128));
    idx_t m, j, k;
    A[0] = 1;
    C[0] = 1; /* row 0 of Pascal's triangle */
    for (m = 1; m <= n; m++) {
        /* C holds row m-1: C[k] = binomial(m-1,k) */
        unsigned __int128 sum = 0, f = 1; /* f = (j-2)! */
        for (j = 2; j <= m; j++) {
            if (j > 2) f *= (unsigned __int128)(j - 2);
            sum += (unsigned __int128)j * f * C[j - 1] * A[m - j];
        }
        A[m] = (unsigned __int128)0 - sum;
        C[m] = 1;
        for (k = m - 1; k >= 1; k--) C[k] += C[k - 1];
    }
    __int128 r = (__int128)A[n];
    free(A);
    free(C);
    return from_i128(r);
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) return a_fixed(n);

    if (n >= memo_len) {
        idx_t newlen = n + 1;
        memo = grow_zero(memo, memo_len, newlen);
        idx_t m;
        for (m = memo_len; m < newlen; m++) {
            if (m == 0) { memo[0] = 1; continue; }
            seq_t P = 1;   /* (m-1)!/(m-j)! */
            seq_t sum = 0;
            idx_t j;
            for (j = 2; j <= m; j++) {
                P *= (seq_t)(m - j + 1);
                seq_t coef = P / (seq_t)(j - 1) * (seq_t)j;
                sum += coef * memo[m - j];
            }
            memo[m] = -sum;
        }
        memo_len = newlen;
    }
    return memo[n];
}

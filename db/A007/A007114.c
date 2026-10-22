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
 * Helpers:   from_i128, grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/from_i128.h"
#include "../utils/grow_zero.h"

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

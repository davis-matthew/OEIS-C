/*
 * A003723  E.g.f. exp(tanh(x)).
 * https://oeis.org/A003723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = P_n(0), where P_0(y) = 1 and P_{n+1}(y) = (1 - y^2) *
 *            (P_n'(y) + P_n(y)), since d^n/dx^n exp(tanh(x)) = P_n(tanh(x)) *
 *            exp(tanh(x))
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 23, i128 n <= 38,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 100 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
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

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

seq_t a(idx_t n) {
    size_t len = (size_t)n + 2;
    if (seq_is_fixed_width()) {
        /* exact modulo 2^128 (ring operations only), so the result is
           correct whenever it fits in seq_t */
        unsigned __int128 *p = (unsigned __int128 *)calloc(len, sizeof(unsigned __int128));
        unsigned __int128 r;
        p[0] = 1;
        for (idx_t s = 1; s <= n; s++) {
            idx_t L = n - s;
            for (idx_t k = 0; k <= L; k++)
                p[k] += (unsigned __int128)(k + 1) * p[k + 1];
            for (idx_t k = L; k >= 2; k--)
                p[k] -= p[k - 2];
        }
        r = p[0];
        free(p);
        return from_i128((__int128)r);
    } else {
        seq_t *p = alloc_zero((idx_t)len);
        seq_t r;
        p[0] = 1;
        for (idx_t s = 1; s <= n; s++) {
            idx_t L = n - s;
            for (idx_t k = 0; k <= L; k++)
                p[k] += p[k + 1] * (seq_t)(k + 1);
            for (idx_t k = L; k >= 2; k--)
                p[k] -= p[k - 2];
        }
        r = p[0];
        release(p, (idx_t)len);
        return r;
    }
}

/*
 * A003725  Expansion of e.g.f. exp( x * exp(-x) ).
 * https://oeis.org/A003725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n} (-k)^(n-k)*binomial(n, k)
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 15, i64 n <= 25, i128 n <= 41,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 556 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

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

typedef unsigned __int128 u128_t;

static u128_t a003725_upow(u128_t b, uint64_t e) {
    u128_t r = 1;
    while (e) {
        if (e & 1) r *= b;
        b *= b;
        e >>= 1;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    if (seq_is_fixed_width()) {
        /* exact modulo 2^128 (wrapping unsigned arithmetic); correct whenever
           the true value fits in seq_t */
        u128_t *row = (u128_t *)malloc((size_t)(n + 1) * sizeof(u128_t));
        idx_t i, j, k;
        u128_t res = 0;
        row[0] = 1;
        for (i = 1; i <= n; i++) {
            row[i] = 1;
            for (j = i - 1; j >= 1; j--) row[j] += row[j - 1];
        }
        for (k = 1; k <= n; k++) {
            u128_t t = a003725_upow((u128_t)k, n - k) * row[k];
            if ((n - k) & 1) res -= t;
            else res += t;
        }
        free(row);
        return from_i128((__int128)res);
    }

    {
        seq_t res = 0;
        seq_t b = 1; /* binomial(n, k) */
        idx_t k;
        for (k = 1; k <= n; k++) {
            b = b * (seq_t)(n - k + 1) / (seq_t)k;
            seq_t t = ipow_idx((seq_t)k, n - k) * b;
            if ((n - k) & 1) res -= t;
            else res += t;
        }
        return res;
    }
}

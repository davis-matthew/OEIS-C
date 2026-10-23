/*
 * A166910  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A166910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies a(0)=1, a(1)=8, … a(12)=15818613944, and for
 *            n≥13: a(n) = 6·∑_{k=1}^{12} a(n‑k) – 21·a(n‑13) + [n==13] . The
 *            indicator [n==13] equals 1 only for n=13 (arising from the
 *            constant term of the generating function)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

static const seq_t init[13] = {
    (seq_t)1,            /* a(0) */
    (seq_t)8,            /* a(1) */
    (seq_t)56,           /* a(2) */
    (seq_t)392,          /* a(3) */
    (seq_t)2744,         /* a(4) */
    (seq_t)19208,        /* a(5) */
    (seq_t)134456,       /* a(6) */
    (seq_t)941192,       /* a(7) */
    (seq_t)6588344,      /* a(8) */
    (seq_t)46118408,     /* a(9) */
    (seq_t)322828856,    /* a(10) */
    (seq_t)2259801992,   /* a(11) */
    (seq_t)15818613944   /* a(12) */
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_len = 0;

    /* enlarge cache if necessary */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = need * 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 13) {
            cache[i] = init[i];
        } else {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 12; ++k) {
                sum += cache[i - k];
            }
            seq_t val = sum * (seq_t)6;
            val -= (seq_t)21 * cache[i - 13];
            if (i == 13) {          /* add the constant term of N(t) */
                val += (seq_t)1;
            }
            cache[i] = val;
        }
    }
    cache_len = (idx_t)n + 1;
    return cache[n];
}

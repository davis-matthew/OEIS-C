/*
 * A169223  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A169223: Number of reduced words of length n in the Coxeter group 
 * on 18 generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 *
 * The ordinary generating function is
 *   G(t) = (t^28 + 2 t^27 + ... + 2 t + 1) /
 *          (136 t^28 - 16 t^27 - ... - 16 t + 1).
 * Expanding this fraction gives a linear recurrence of order 28:
 *
 *   a[n] + 16 · (a[n-1] + a[n-2] + ... + a[n-27]) - 136 · a[n-28] = p[n]
 *
 * where p[n] = 1 for n = 0, 2 for 1 ≤ n ≤ 27, 1 for n = 28, and 0 for n > 28.
 * The recurrence is applied for all n ≥ 0; for n < 28 the terms a[0]..a[27]
 * are produced automatically by the same formula (no division is required).
 *
 * A memoised linear‑time implementation is provided below.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_small.h"          /* seq_mul_small(seq_t x, unsigned int m) */

static seq_t *cache = NULL;
static idx_t  capacity = 0;
static idx_t  size = 0;      /* number of values stored (cache[0] .. cache[size-1]) */

/* coefficient p[n] */
static inline seq_t p_value(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n <= 27) return (seq_t)2;
    if (n == 28) return (seq_t)1;
    return (seq_t)0;
}

/* ensure cache has room for indices up to n inclusive */
static void ensure_capacity(idx_t n)
{
    if (n < size) return;
    idx_t new_cap = capacity ? capacity * 2 : 64;
    while (new_cap <= n) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();             /* out of memory */
    cache = new_cache;
    capacity = new_cap;
}

/* main function required by the build */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* initialise cache[0] if needed */
    if (size == 0) {
        cache[0] = (seq_t)1;   /* a(0) */
        size = 1;
    }

    /* compute missing terms */
    if (size - 1 < n) {
        /* compute sliding sum of last up to 27 terms */
        seq_t sumPrev = 0;
        if (size > 0) {
            idx_t start = size > 27 ? size - 27 : 0;
            for (idx_t j = start; j < size; ++j)
                sumPrev += cache[j];
        }

        for (idx_t i = size; i <= n; ++i) {
            seq_t term = p_value(i);
            term += mul_small(sumPrev, 16);
            if (i >= 28)
                term -= mul_small(cache[i - 28], 136);
            cache[i] = term;

            /* update sliding window for next iteration */
            sumPrev += cache[i];
            if (i >= 27)
                sumPrev -= cache[i - 27];
        }
        size = n + 1;
    }

    return cache[n];
}

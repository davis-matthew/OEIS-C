/*
 * A164688  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A164688
 *  a(0) = 1, a(1) = 44, a(2) = 1892, a(3) = 81356,
 *  a(4) = 3498308, a(5) = 150427244, a(6) = 6468371492
 *  Recurrence (derived from the denominator of the generating function):
 *      a(n) = 42·a(n‑1) + 42·a(n‑2) + 42·a(n‑3) +
 *             42·a(n‑4) + 42·a(n‑5) + 42·a(n‑6)
 *           – 903·a(n‑7)            for n ≥ 8
 *      a(7) = 42·a(6)+42·a(5)+42·a(4)+42·a(3)+42·a(2)+42·a(1)
 *             – 903·a(0) + 1
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;          /* cache[n] = a(n) */
static size_t cache_size = 0;        /* allocated slots (≥ cache_computed) */
static size_t cache_computed = 0;    /* number of entries already filled */

/* Ensure the cache has at least (n+1) slots.  */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_size)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp) {
        abort(); /* out of memory – not expected in tests */
    }
    cache = tmp;
    cache_size = needed;
}

seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Base cases.  */
    static const seq_t init_vals[7] = {
        (seq_t)1,
        (seq_t)44,
        (seq_t)1892,
        (seq_t)81356,
        (seq_t)3498308,
        (seq_t)150427244,
        (seq_t)6468371492
    };

    /* If the required term is already computed, return it. */
    if (n < (idx_t)cache_computed) {
        return cache[(size_t)n];
    }

    /* Fill the cache up to index n. */
    for (size_t i = cache_computed; i <= (size_t)n; ++i) {
        if (i < 7) {
            cache[i] = init_vals[i];
            continue;
        }

        /* Sum of the previous six terms. */
        seq_t sum = (seq_t)0;
        sum += cache[i - 1];
        sum += cache[i - 2];
        sum += cache[i - 3];
        sum += cache[i - 4];
        sum += cache[i - 5];
        sum += cache[i - 6];

        seq_t term = mul_small(sum, 42u);                  /* 42 * sum */
        seq_t subtract = mul_small(cache[i - 7], 903u);    /* 903 * a(i-7) */
        term -= subtract;                                 /* term = 42*sum - 903*a(i-7) */

        if (i == 7) term += (seq_t)1;                     /* +1 for n==7 */

        cache[i] = term;
    }

    cache_computed = (size_t)n + 1;
    return cache[(size_t)n];
}

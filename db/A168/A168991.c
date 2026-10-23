/*
 * A168991  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A168991
 *
 * The sequence satisfies the linear recurrence
 *
 *   a(0) = 1
 *   a(1) = 26
 *   a(2) = 650
 *   a(3) = 16250
 *   ...
 *
 *   a(n) = 24·(a(n-1)+ … + a(n-22)) – 300·a(n-23)   for n ≥ 24
 *
 * The first 23 terms (0–22) are obtained from the generating
 * function using its numerator.  After a(23) the above recurrence
 * is used for all subsequent indices.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* size allocated */
    static idx_t computed_upto = 0;   /* next index to compute (cache[0..computed_upto-1] known) */
    static seq_t sum_last22 = 0;     /* sum of the 22 terms used for the recurrence */

    /* Grow cache if required */
    if (cache == NULL || cache_size < n + 1) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { /* allocation failure – return zero */
            if (cache) free(cache);
            cache = NULL;
            cache_size = 0;
            computed_upto = 0;
            return (seq_t)0;
        }
        cache = tmp;
        /* initialize new entries */
        for (idx_t i = cache_size; i < new_size; ++i) cache[i] = 0;
        cache_size = new_size;
    }

    /* Compute terms up to n */
    while (computed_upto <= n) {
        if (computed_upto <= 22) {
            /* compute a[0] … a[22] from the numerator */
            seq_t base = (computed_upto == 0 ? (seq_t)1 : (seq_t)2);
            seq_t s = 0;
            for (idx_t k = 1; k <= computed_upto; ++k) s += cache[computed_upto - k];
            cache[computed_upto] = base + (seq_t)24 * s;
            ++computed_upto;
        } else if (computed_upto == 23) {
            /* compute a[23] using the extra '+1' term in the numerator */
            seq_t total_sum = 0;
            for (idx_t i = 1; i <= 22; ++i) total_sum += cache[i];
            cache[23] = (seq_t)24 * total_sum - (seq_t)299;   /* 24*Σ a[1..22] – 299 */
            /* prepare sum of 22 terms for the next recurrence step:
               sum now = a[23] + a[22] + … + a[2] */
            sum_last22 = total_sum + cache[23] - cache[1];
            ++computed_upto;
        } else {
            /* recurrent computation for n ≥ 24 */
            idx_t i = computed_upto;          /* index being computed */
            seq_t val = (seq_t)24 * sum_last22 - (seq_t)300 * cache[i - 23];
            cache[i] = val;
            /* update running sum: drop a[i-22], add a[i] */
            sum_last22 += val - cache[i - 22];
            ++computed_upto;
        }
    }

    return cache[n];
}

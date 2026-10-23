/*
 * A041395  Denominators of continued fraction convergents to sqrt(212).
 * https://oeis.org/A041395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(212) */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* partial quotients for sqrt(212): a0 = 14, a1..a14 = {1,1,3,1,1,1,6,1,1,1,3,1,1,28} */
static const seq_t periodic[15] = {14,1,1,3,1,1,1,6,1,1,1,3,1,1,28};

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;            /* q0 = 1 */

    if (cache_size <= n) {
        size_t new_size = n + 1;            /* include index 0..n */
        seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        /* base case */
        new_cache[0] = (seq_t)1;            /* q0 = 1 */

        /* set up previous two denominators for the recurrence */
        seq_t q_prev_minus1 = 0;            /* q_{-1} = 0 */
        seq_t q_prev = 1;                  /* q0 = 1 */
        idx_t start = 1;

        if (cache_size >= 2) {             /* we have at least q_{k-2} and q_{k-1} cached */
            q_prev_minus1 = cache[cache_size-2];
            q_prev = cache[cache_size-1];
            start = cache_size;
            /* copy older cached values */
            for (idx_t i = 1; i < cache_size; ++i) new_cache[i] = cache[i];
        } else if (cache_size == 1) {       /* only q0 cached */
            q_prev_minus1 = 0;
            q_prev = cache[0];
            start = 1;
        }

        /* extend cache up to index n */
        for (idx_t i = start; i <= n; ++i) {
            idx_t r = i % 14;
            if (r == 0) r = 14;
            seq_t a_i = periodic[r];
            seq_t q_curr = a_i * q_prev + q_prev_minus1;
            new_cache[i] = q_curr;
            q_prev_minus1 = q_prev;
            q_prev = q_curr;
        }

        free(cache);
        cache = new_cache;
        cache_size = (idx_t)new_size;
    }

    return cache[n];
}

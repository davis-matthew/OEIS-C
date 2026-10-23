/*
 * A042259  Denominators of continued fraction convergents to sqrt(655).
 * https://oeis.org/A042259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominators of convergents to sqrt(655) Recurrence for
 *            n>31: a(n) = 1475418418*a(n-16) - a(n-32) Initial terms
 *            a(0)…a(31) are computed from the continued fraction expansion
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoized sequence values */
static idx_t cache_capacity = 0;     /* number of allocated slots */
static idx_t cache_size = 0;         /* number of computed terms */

/* partial quotients of the continued fraction of sqrt(655)
   after the initial 25, the repeating block of length 16 is: */
static const int period[16] = {
    1, 1, 2, 5, 3, 2, 8, 10, 8, 2, 3, 5, 2, 1, 1, 50
};

seq_t a(idx_t n)
{
    /* initialize the cache on first call */
    if (cache == NULL) {
        /* 32 slots are enough for the first 32 terms */
        cache_capacity = 32;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;                /* allocation failure */

        /* compute the first 32 terms from the continued fraction */
        seq_t p_prev2 = 0;   /* p_{-2} */
        seq_t p_prev1 = 1;   /* p_{-1} */
        seq_t q_prev2 = 1;   /* q_{-2} */
        seq_t q_prev1 = 0;   /* q_{-1} */

        for (idx_t idx = 0; idx < 32; ++idx) {
            int a_k;
            if (idx == 0)
                a_k = 25;                           /* a0 */
            else if (idx == 16)
                a_k = 50;                           /* a16 */
            else
                a_k = period[(idx - 1) % 16];        /* a1 … a15 and repeats */

            seq_t p_cur = (seq_t)a_k * p_prev1 + p_prev2;
            seq_t q_cur = (seq_t)a_k * q_prev1 + q_prev2;

            cache[idx] = q_cur;                    /* store denominator */

            /* shift for next iteration */
            p_prev2 = p_prev1;  p_prev1 = p_cur;
            q_prev2 = q_prev1;  q_prev1 = q_cur;
        }
        cache_size = 32;
    }

    /* if the requested n is already computed, return it */
    if (n < cache_size)
        return cache[n];

    /* extend the cache up to n using the linear recurrence */
    while (cache_size <= n) {
        if (cache_size == cache_capacity) {
            idx_t new_capacity = cache_capacity ? cache_capacity * 2 : 32;
            seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
            if (tmp == NULL)
                return (seq_t)0;                    /* allocation failure */
            cache = tmp;
            cache_capacity = new_capacity;
        }

        /* recurrence: a(n) = 1475418418*a(n-16) - a(n-32)  (n > 31) */
        seq_t val = ((seq_t)1475418418) * cache[cache_size - 16]
                        - cache[cache_size - 32];
        cache[cache_size] = val;
        ++cache_size;
    }

    return cache[n];
}

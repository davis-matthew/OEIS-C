/*
 * A042729  Denominators of continued fraction convergents to sqrt(894).
 * https://oeis.org/A042729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(894). */
/* Recurrence: q_{-2}=1, q_{-1}=0; q_n = a_n*q_{n-1} + q_{n-2}. */
/* Partial quotients: a_0 = 29, then periodic pattern [1,8,1,58]. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached results */
    static size_t cache_cap = 0;     /* capacity of cache array */
    static size_t comp_size = 0;     /* number of values stored (0 .. comp_size-1) */

    /* Make sure the cache can hold up to index n */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < required) new_cap <<= 1;
        /* cast result of realloc for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If we already have the value, return it */
    if ((size_t)n < comp_size) return cache[n];

    /* Set up previous two denominators depending on what is already known */
    seq_t q_prev1; /* q_{i-1} */
    seq_t q_prev2; /* q_{i-2} */
    size_t i_start;

    if (comp_size == 0) {          /* no terms computed yet */
        q_prev1 = 0;  /* q_{-1} */
        q_prev2 = 1;  /* q_{-2} */
        i_start = 0;
    } else if (comp_size == 1) {   /* only q_0 computed */
        q_prev1 = cache[0];        /* q_0 */
        q_prev2 = 0;               /* q_{-1} */
        i_start = 1;
    } else {                       /* at least q_{comp_size-2} and q_{comp_size-1} known */
        q_prev1 = cache[comp_size - 1];
        q_prev2 = cache[comp_size - 2];
        i_start = comp_size;
    }

    /* Run the recurrence until we reach the requested index */
    for (size_t i = i_start; i <= (size_t)n; ++i) {
        seq_t a_i;
        if (i == 0) {
            a_i = 29;
        } else {
            size_t idx = (i - 1) % 4;
            if (idx == 0) a_i = 1;
            else if (idx == 1) a_i = 8;
            else if (idx == 2) a_i = 1;
            else a_i = 58;
        }

        seq_t q_i = a_i * q_prev1 + q_prev2;
        cache[i] = q_i;
        q_prev2 = q_prev1;
        q_prev1 = q_i;
    }

    comp_size = required;
    return cache[n];
}

/*
 * A041307  Denominators of continued fraction convergents to sqrt(166).
 * https://oeis.org/A041307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for denominators of convergents of sqrt(166).
   Let a0 = 12 and for n >= 1 let a_n be periodic with period
   [1, 7, 1, 1, 1, 2, 4, 1, 3, 2, 12, 2, 3, 1, 4, 2, 1, 1, 1, 7, 1, 24].
   Define q_{-2} = 1, q_{-1} = 0 and
       q_n = a_n * q_{n-1} + q_{n-2}   (n >= 0).
   Then a(n) = q_n.  First terms: 1, 1, 8, 9, 17, 26, 69, … */

#include <stdlib.h>

/* Periodic partial quotients for n >= 1 */
static const seq_t period[22] = {
    (seq_t)1, (seq_t)7, (seq_t)1, (seq_t)1, (seq_t)1,
    (seq_t)2, (seq_t)4, (seq_t)1, (seq_t)3, (seq_t)2,
    (seq_t)12, (seq_t)2, (seq_t)3, (seq_t)1, (seq_t)4,
    (seq_t)2, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)7,
    (seq_t)1, (seq_t)24
};

static seq_t *cache    = NULL;   /* cached values a(0 .. cache_size-1) */
static idx_t  cache_size = 0;    /* number of terms stored          */
static size_t cache_cap  = 0;    /* current allocated capacity      */

static void ensure_cache(idx_t n)
{
    idx_t target = n + 1;
    if (cache_size >= target) return;           /* nothing to do */

    /* increase allocation if necessary */
    if (cache_cap < target) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap;
        while (new_cap < (size_t)target) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise previous two denominators */
    seq_t prev1, prev2;                          /* prev1 = q_{i-1}, prev2 = q_{i-2} */
    if (cache_size == 0) {
        prev1 = 0;  /* q_{-1} */
        prev2 = 1;  /* q_{-2} */
    } else if (cache_size == 1) {
        prev1 = cache[0]; /* q_0 */
        prev2 = 0;        /* q_{-1} */
    } else {
        prev1 = cache[cache_size - 1]; /* q_{last} */
        prev2 = cache[cache_size - 2]; /* q_{second_last} */
    }

    /* compute new terms up to index n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t a_i;
        if (i == 0)
            a_i = (seq_t)12;                     /* a_0 */
        else
            a_i = period[(size_t)((i - 1) % 22)]; /* periodic part */

        seq_t q_i = a_i * prev1 + prev2;          /* recurrence */
        cache[i] = q_i;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = q_i;
    }

    cache_size = target;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}

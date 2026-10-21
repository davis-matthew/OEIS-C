/*
 * A089039  Number of circular permutations of 2n letters that are free of
 *          jealousy.
 * https://oeis.org/A089039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for sequence A089039 (OEIS).
   Base values: a(1)=1, a(2)=2, a(3)=6, a(4)=60, a(5)=960.
   For n ≥ 2 (i = n+4) :
        a(i) = (i-1)(i-2) * [ 2·a(i-1) - ( (i-4)(i-3)a(i-4) + 2(i-3)²a(i-3)
                                + (i-4)(i-1)a(i-2) ) ].
   This follows from the OEIS recurrence
        a(n+4) = -(n+3)(n+2)[ n(n+1)a(n)+2(n+1)²a(n+1)+n(n+3)a(n+2)-2a(n+3) ]. */

#include <stdlib.h>

/* Static cache of computed terms.  cache[1] .. cache[cache_len] are valid. */
static seq_t *cache = NULL;        /* 1‑based array */
static idx_t cache_len  = 0;       /* highest index computed */
static idx_t cache_size = 0;       /* allocated size (≥ cache_len+1) */

static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;                      /* already available */

    /* Grow the cache if necessary. */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* Initialise the first five values if they were not set already. */
    for (idx_t i = cache_len + 1; i <= 5 && i <= n; ++i) {
        switch (i) {
            case 1: cache[1] = (seq_t)1;  break;
            case 2: cache[2] = (seq_t)2;  break;
            case 3: cache[3] = (seq_t)6;  break;
            case 4: cache[4] = (seq_t)60; break;
            case 5: cache[5] = (seq_t)960; break;
        }
    }

    /* Compute terms from the recurrence (valid for i >= 6). */
    idx_t start = cache_len + 1;
    if (start < 6) start = 6;

    for (idx_t i = start; i <= n; ++i) {
        /* a(i-4), a(i-3), a(i-2), a(i-1) are already computed. */
        seq_t a_ip4 = cache[i - 4];
        seq_t a_ip3 = cache[i - 3];
        seq_t a_ip2 = cache[i - 2];
        seq_t a_ip1 = cache[i - 1];

        /* Compute the components of the recurrence. */
        seq_t part1 = (seq_t)(i - 4) * (seq_t)(i - 3) * a_ip4;
        seq_t part2 = (seq_t)2 * (seq_t)(i - 3) * (seq_t)(i - 3) * a_ip3;
        seq_t part3 = (seq_t)(i - 4) * (seq_t)(i - 1) * a_ip2;
        seq_t part4 = (seq_t)2 * a_ip1;

        /* part4 - (part1 + part2 + part3) is always positive for i ≥ 6. */
        seq_t sub   = part1 + part2 + part3;
        seq_t diff  = part4 - sub;

        seq_t coeff = (seq_t)(i - 1) * (seq_t)(i - 2);

        cache[i] = coeff * diff;
    }

    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* below the defined offset */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}

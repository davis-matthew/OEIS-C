/*
 * A208265  Number of n X 4 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or left-upward diagonal neighbors.
 * https://oeis.org/A208265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) + 20*a(n-2) - 21*a(n-3) - 30*a(n-4) + 8*a(n-5)
 *            Initial values: a(1)=8, a(2)=89, a(3)=1033, a(4)=11929,
 *            a(5)=137845
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* stores computed values, 1‑based index offset */
static idx_t cache_len = 0;     /* number of values stored */
static size_t cache_cap = 0;    /* allocated capacity */

/* Compute the nth term of OEIS A208265 */
seq_t a(idx_t n)
{
    /* initialise first five terms on first use */
    if (cache_len == 0) {
        /* allocate space for at least five values */
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)8;          /* a(1) */
        cache[1] = (seq_t)89;         /* a(2) */
        cache[2] = (seq_t)1033;       /* a(3) */
        cache[3] = (seq_t)11929;      /* a(4) */
        cache[4] = (seq_t)137845;     /* a(5) */
        cache_len = 5;
    }

    /* if the desired term has already been computed, return it */
    if (n <= cache_len)
        return cache[n-1];

    /* extend the cache up to the needed position */
    while (cache_len < n) {
        /* ensure enough capacity */
        if (cache_cap < cache_len + 1) {
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* recurrence calculation */
        seq_t a1 = cache[cache_len-1];                 /* a(k-1) */
        seq_t a2 = cache[cache_len-2];                 /* a(k-2) */
        seq_t a3 = cache[cache_len-3];                 /* a(k-3) */
        seq_t a4 = cache[cache_len-4];                 /* a(k-4) */
        seq_t a5 = cache[cache_len-5];                 /* a(k-5) */

        seq_t part1 = 10 * a1;
        part1 += 20 * a2;
        part1 += 8 * a5;

        seq_t part2 = 21 * a3;
        part2 += 30 * a4;

        cache[cache_len] = part1 - part2;              /* a(k) */
        cache_len++;
    }

    return cache[n-1];
}

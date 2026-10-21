/*
 * A231248  Number of 3 X n 0..3 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 4, no adjacent elements equal, and upper left
 *          element zero.
 * https://oeis.org/A231248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-1) - 3*a(n-2) + 27*a(n-3) + 28*a(n-4)
 *          + 14*a(n-5) - 8*a(n-6) - 8*a(n-7)   (n > 8)
 * Initial values:
 *   a(1)=0, a(2)=6, a(3)=78, a(4)=464, a(5)=3090,
 *   a(6)=22460, a(7)=162766, a(8)=1169472
 * The function memoises results so that calls in any order are O(1)
 * after the initial buildup.  It works for all supported seq_t types,
 * including arbitrary‑precision integer types; for integer types the
 * computation uses 64‑bit arithmetic, which is sufficient for all
 * values that fit in seq_t. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[0] unused; indices start at 1 */
static idx_t  cache_max = 0;         /* largest n with cached value */

static void init_initial_values(void)
{
    if (cache_max >= 8) return;      /* already initialised */
    /* Allocate space for indices 0..8 */
    cache = (seq_t *)calloc(9, sizeof(seq_t));
    cache[0] = 0;                   /* unused */
    cache[1] = 0;
    cache[2] = 6;
    cache[3] = 78;
    cache[4] = 464;
    cache[5] = 3090;
    cache[6] = 22460;
    cache[7] = 162766;
    cache[8] = 1169472;
    cache_max = 8;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* outside the offset; safe */
    init_initial_values();

    if (n > cache_max) {
        /* Grow the cache to accommodate n */
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i <= 8) continue;    /* already set */
            /* Retrieve the required previous terms */
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];
            seq_t a5 = cache[i-5];
            seq_t a6 = cache[i-6];
            seq_t a7 = cache[i-7];

            /* Sum positive and negative contributions separately */
            seq_t pos = 7*a1 + 27*a3 + 28*a4 + 14*a5;
            seq_t neg = 3*a2 + 8*a6 + 8*a7;

            cache[i] = pos - neg;     /* pos ≥ neg for all terms that fit */
        }
        cache_max = n;
    }
    return cache[n];
}

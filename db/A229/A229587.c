/*
 * A229587  Number of defective 3-colorings of a 2 X n 0..2 array connected
 *          horizontally and antidiagonally with exactly one mistake, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=6, a(3)=28, a(4)=116; a(n) = 6*a(n-1) - 9*a(n-2)
 *            for n>4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] holds a(1) */
static idx_t max_cached = 0;   /* number of terms currently cached */

static void ensure_cache(idx_t n)
{
    if (n <= max_cached) return;

    /* Grow the cache to hold n terms */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    /* Initialise the initial known values if not already done */
    if (max_cached < 1 && n >= 1) cache[0] = 0;          /* a(1) */
    if (max_cached < 2 && n >= 2) cache[1] = 6;          /* a(2) */
    if (max_cached < 3 && n >= 3) cache[2] = 28;         /* a(3) */
    if (max_cached < 4 && n >= 4) cache[3] = 116;        /* a(4) */

    /* Compute new terms using the recurrence */
    idx_t start = max_cached + 1;
    if (start < 5) start = 5;        /* recurrence starts at n=5 */
    for (idx_t i = start; i <= n; ++i) {
        seq_t t1 = cache[i-2];   /* a(i-1) */
        t1 *= 6;
        seq_t t2 = cache[i-3];   /* a(i-2) */
        t2 *= 9;
        cache[i-1] = t1 - t2;    /* a(i) */
    }

    max_cached = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range, not expected */
    ensure_cache(n);
    return cache[n-1];
}

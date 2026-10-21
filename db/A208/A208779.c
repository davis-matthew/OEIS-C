/*
 * A208779  Number of n X 7 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 1 and 1 1 1 vertically.
 * https://oeis.org/A208779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=42, a(2)=1764, a(3)=252, a(4)=420, a(n)=a(n-1)+a(n-2) for
 *            n>4
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 37, u64 n <= 83, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;   /* 1‑based array, index 0 unused */
static idx_t  cache_size = 0; /* largest index currently stored */

/* Return the nth term of OEIS sequence A208779 */
seq_t a(idx_t n)
{
    if (n == 0) /* behaviour below the offset is undefined */
        return 0;

    /* Allocate and initialise cache if not already done */
    if (cache == NULL) {
        /* capacity up to 4 is sufficient for the initial terms */
        cache_size = 4;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        if (cache == NULL)
            return 0; /* out of memory, caller must handle */
        cache[1] = (seq_t)42;
        cache[2] = (seq_t)1764;
        cache[3] = (seq_t)252;
        cache[4] = (seq_t)420;
    }

    /* If the requested term is already cached, return it */
    if (n <= cache_size)
        return cache[n];

    /* Extend cache to hold up to n */
    idx_t new_size = n;
    seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
    if (tmp == NULL)
        return 0; /* out of memory */
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i)
        cache[i] = cache[i - 1] + cache[i - 2];

    cache_size = n;
    return cache[n];
}

/*
 * A116753  Number of permutations of length n which avoid the patterns 2134,
 *          3241, 4312.
 * https://oeis.org/A116753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 52*a(n-2) + 138*a(n-3) - 225*a(n-4) +
 *            231*a(n-5) - 146*a(n-6) + 52*a(n-7) - 8*a(n-8) (valid for n ≥ 9)
 *            Initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=73,
 *            a(6)=236, a(7)=705, a(8)=1970
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 25, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;       /* Array of computed a(n), index by n */
static idx_t  cap   = 0;          /* Capacity of cache (number of elements) */
static idx_t  last  = 0;          /* Highest n that has been computed */

/* Initialise cache with the first 8 terms. */
static void init_cache(void)
{
    if (cache != NULL)          /* already initialised */
        return;

    /* Allocate space for indices 0..8 (cache[0] unused) */
    cap  = 9;
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cache)
        exit(1);                /* fatal: out of memory */

    /* dummy entry for index 0 */
    cache[0] = 0;
    cache[1] = 1;
    cache[2] = 2;
    cache[3] = 6;
    cache[4] = 21;
    cache[5] = 73;
    cache[6] = 236;
    cache[7] = 705;
    cache[8] = 1970;

    last = 8;
}

/* Expand the cache to at least n (inclusive). */
static void extend_cache(idx_t n)
{
    if (n <= last)
        return;

    /* Grow capacity if necessary. */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        while ((size_t)(n + 1) > new_cap)
            new_cap <<= 1;               /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute terms nalon n-1 down to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* recurrence valid for i ≥ 9; for i ≤ 8 values are already known */
        seq_t t = 11 * cache[i - 1]
                - 52 * cache[i - 2]
                + 138 * cache[i - 3]
                - 225 * cache[i - 4]
                + 231 * cache[i - 5]
                - 146 * cache[i - 6]
                + 52 * cache[i - 7]
                - 8 * cache[i - 8];
        cache[i] = t;
    }
    last = n;
}

/* Public function computing the nth term of A116753. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but return 0 for safety */

    init_cache();     /* ensure starting values for 1..8 are present */

    extend_cache(n);  /* compute up to n if necessary */

    return cache[n];
}

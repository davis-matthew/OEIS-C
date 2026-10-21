/*
 * A095307  Number of walks of length n between two nodes at distance 2 in the
 *          cycle graph C_7.
 * https://oeis.org/A095307
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 3*a(n-3) - 2*a(n-4) Initial values:
 *            a(2) = 1, a(3) = 0, a(4) = 4, a(5) = 1 (a(0) = a(1) = 0 for
 *            internal use)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *cache = NULL;      /* cache[n] = a(n)   */
static idx_t cache_size = 0;     /* allocated size (indices 0 … cache_size-1) */
static idx_t computed_upto = 1;  /* highest index whose value is valid */

/* Allocate or enlarge the cache to hold indices 0 … n */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_size)
        return;
    size_t new_size = cache_size > 0 ? cache_size : 1;
    while ((size_t)n + 1 > new_size)
        new_size <<= 1;          /* double until big enough */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();           /* allocation failure */
    cache = tmp;
    cache_size = new_size;
}

/* Initialise the first few known terms of the sequence */
static void init_cache(void)
{
    if (computed_upto >= 5)  /* already initialised */
        return;
    ensure_cache(5);          /* indices 0 … 5 are needed */

    /* Known values: a(0) = a(1) = 0 are only for recurrence safety */
    cache[0] = 0;
    cache[1] = 0;
    cache[2] = 1;   /* a(2) */
    cache[3] = 0;   /* a(3) */
    cache[4] = 4;   /* a(4) */
    cache[5] = 1;   /* a(5) */

    computed_upto = 5;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;     /* values below the offset are undefined */

    init_cache();         /* ensure base terms are set */

    if ((size_t)n + 1 > cache_size)
        ensure_cache(n); /* grow cache if needed */

    if (n > computed_upto) {
        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            seq_t term = cache[i-1];
            term += 4 * cache[i-2];
            term -= 3 * cache[i-3];
            term -= 2 * cache[i-4];
            cache[i] = term;
        }
        computed_upto = n;
    }

    return cache[n];
}

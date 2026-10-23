/*
 * A021379  Expansion of 1/((1-x)*(1-3*x)*(1-4*x)*(1-9*x)).
 * https://oeis.org/A021379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A021379:  a(0)=1, a(1)=17, a(2)=198, a(3)=2002;
 * for n>3  a(n) = 17*a(n-1) - 91*a(n-2) + 183*a(n-3) - 108*a(n-4)   */
#include <stdlib.h>   /* malloc, realloc, free */

static seq_t *cache = NULL;      /* cache of computed terms   */
static idx_t  cached_len = 0;    /* largest n cached (0‑based) */
static size_t cached_cap = 0;    /* current capacity of cache */

/* Ensure cache has capacity for indices 0..n (inclusive).  The
   cache is at least 4 slots large so we can store the first four terms. */
static void ensure_capacity(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required < 4)
        required = 4;
    if (required > cached_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            exit(1);          /* allocation failure */
        }
        cache = tmp;
        cached_cap = required;
    }
}

/* Initialise first four terms once.  This is safe because ensure_capacity
   guarantees we have at least four slots. */
static void init_cache(void)
{
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)17;
    cache[2] = (seq_t)198;
    cache[3] = (seq_t)2002;
    cached_len = 3;            /* indices 0..3 are stored */
}

seq_t a(idx_t n)
{
    /* make sure we have room for index n (and at least 4 slots) */
    ensure_capacity(n);

    /* lazily initialise the first four terms once */
    if (cached_len == 0)
        init_cache();

    /* if already cached, return it */
    if (n <= cached_len)
        return cache[(size_t)n];

    /* compute missing terms up to n using the 4‑term recurrence */
    for (idx_t k = cached_len + 1; k <= n; ++k) {
        cache[(size_t)k] =
            (seq_t)17  * cache[(size_t)(k - 1)]
          - (seq_t)91  * cache[(size_t)(k - 2)]
          + (seq_t)183 * cache[(size_t)(k - 3)]
          - (seq_t)108 * cache[(size_t)(k - 4)];
    }
    cached_len = n;
    return cache[(size_t)n];
}

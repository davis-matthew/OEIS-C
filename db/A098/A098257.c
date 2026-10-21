/*
 * A098257  Chebyshev polynomials S(n,531).
 * https://oeis.org/A098257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 531*a(n-1) - a(n-2), a(0)=1, a(1)=531
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache storage for memoised terms */
static seq_t *cache   = NULL;   /* array of terms */
static size_t cap     = 0;      /* allocated capacity */
static size_t used    = 0;      /* largest index + 1 that has been computed */

/* Ensure the cache can store terms up to index n */
static void extend_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cap)   /* already big enough */
        return;

    size_t new_cap = cap ? cap : 2;
    while (new_cap < need)
        new_cap <<= 1;               /* grow geometrically */

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    cache = new_cache;
    cap   = new_cap;
}

/* Initialise the first two terms if they haven't been set yet */
static void init_base(void)
{
    if (used >= 2) return;          /* already initialised */

    extend_cache(1);                /* need room for indices 0 and 1 */

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)531;
    used    = 2;
}

/* Return a(n) with memoisation */
seq_t a(idx_t n)
{
    /* Handle the trivial cases immediately to avoid any cache access */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)531;

    extend_cache(n);         /* ensure room for index n */
    init_base();             /* make sure 0 and 1 are defined */

    /* Compute any missing terms up to n */
    for (size_t i = used; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] * (seq_t)531 - cache[i-2];
    }
    used = (size_t)n + 1;    /* mark indices up to n as computed */

    return cache[n];
}

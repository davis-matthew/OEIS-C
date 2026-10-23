/*
 * A020877  Number of matchings in Moebius ladder M_n.
 * https://oeis.org/A020877
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - a(n-4) for n >= 6 Initial values
 *            (offset 2): a(2)=10, a(3)=34, a(4)=106, a(5)=344
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* dynamic cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;       /* allocated size of cache */
static idx_t max_computed = 1;     /* highest n that has been computed (>=1) */

/* initialise cache with base values up to n = 5 */
static void init_cache(void)
{
    if (!cache) {
        cache_cap = 6;                      /* indices 0 .. 5 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return;                        /* out of memory – caller will see 0 */

        cache[0] = (seq_t)0;               /* unused for n < 2 */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)10;               /* a(2) */
        cache[3] = (seq_t)34;               /* a(3) */
        cache[4] = (seq_t)106;              /* a(4) */
        cache[5] = (seq_t)344;              /* a(5) */

        max_computed = 5;
    }
}

/* ensure the cache can hold up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)                /* out of memory – keep old cache */
        return;
    cache = new_cache;
    cache_cap = new_cap;
}

/* return a(n) for n >= 2, memoising results */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;           /* undefined below offset – return 0 */

    init_cache();                  /* set up base values */

    if (n > max_computed) {
        ensure_capacity(n);
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* recurrence uses a(i-1), a(i-2) and a(i-4); base values exist */
            cache[i] = 2 * cache[i - 1] + 4 * cache[i - 2] - cache[i - 4];
        }
        max_computed = n;
    }
    return cache[n];
}

/*
 * A229137  a(1) = a(2) = 1; if n == 0 (mod 3), then a(n) = a(n/3), otherwise
 *          a(n) = a(n-1) + a(n-2).
 * https://oeis.org/A229137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 1; if n % 3 == 0 then a(n) = a(n/3) else a(n) =
 *            a(n-1) + a(n-2) (OEIS A229137)
 * Range:     u8 n <= 63, u16 n <= 363, u32 n <= 3804, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>       /* for size_t */

static seq_t *cache   = NULL;  /* cache[0] is unused, cache[1] = a(1) */
static size_t cache_size = 0;  /* current number of allocated slots */

/* Ensure cache contains values up to and including n */
static void ensure_cache(idx_t n)
{
    if (n < 1) return;                /* nothing to do for invalid n */

    size_t sz = (size_t)n + 1;        /* +1 because we want index n inclusive */
    if (sz <= cache_size) return;     /* already have enough space */

    /* Reallocate cache (cast required for C++ compatibility) */
    seq_t *new_cache = (seq_t *)realloc(cache, sz * sizeof(seq_t));
    if (!new_cache) exit(1);          /* allocation failure */
    cache      = new_cache;
    cache_size = sz;

    /* Compute any new values */
    for (size_t i = 1; i < sz; ++i)
    {
        if (i == 1 || i == 2) {
            cache[i] = 1;
        } else if (i % 3 == 0) {
            /* i/3 is guaranteed to be < i, so its value exists */
            cache[i] = cache[i / 3];
        } else {
            /* a(i-1) and a(i-2) are already computed */
            cache[i] = cache[i - 1] + cache[i - 2];
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;    /* undefined for n < 1, return 0 as sentinel */
    ensure_cache(n);
    return cache[(size_t)n];
}

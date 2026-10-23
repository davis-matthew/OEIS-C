/*
 * A030517  Number of walks of length n between two vertices on an icosahedron
 *          at distance 1.
 * https://oeis.org/A030517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 4*a(n-1) + 10*a(n-2) − 20*a(n-3) − 25*a(n-4) for n ≥ 5,
 *            with initial values a(1)=1, a(2)=2, a(3)=13, a(4)=52
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A thread‑safe, order‑agnostic implementation using a growing static
   cache.  The cache is initialized on first use and extends to the
   requested index n, computing any missing terms on demand.  All
   arithmetic is performed in seq_t, which may be a 8‑, 16‑, 32‑, 64‑ or
   128‑bit unsigned integer, or an arbitrary‑precision integer type. */

#include <stdlib.h>    /* malloc, realloc, abort */

static seq_t *cache = NULL;       /* cache[1..last] */
static idx_t  last  = 0;          /* largest n already computed */

/* Ensure the cache can hold indices up to `need` (inclusive).
   `need` is at least 4, so the allocation size is `need + 1`. */
static void
grow_cache(idx_t need)
{
    idx_t   new_size = need + 1;            /* include dummy cache[0] */
    size_t  byte_size = (size_t)new_size * sizeof(seq_t);
    seq_t * new_ptr;

    if (cache == NULL) {
        new_ptr = (seq_t *)malloc(byte_size);
    } else {
        new_ptr = (seq_t *)realloc(cache, byte_size);
    }
    if (!new_ptr) {
        abort();          /* allocation failure */
    }
    cache = new_ptr;
    /* If this is the first allocation, set base values. */
    if (last == 0) {
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 13;
        cache[4] = 52;
        last = 4;
    }
    /* Compute any missing terms up to `need`. */
    for (idx_t i = last + 1; i <= need; ++i) {
        seq_t t1 = 4 * cache[i - 1];
        seq_t t2 = 10 * cache[i - 2];
        seq_t t3 = 20 * cache[i - 3];
        seq_t t4 = 25 * cache[i - 4];
        cache[i] = t1 + t2 - t3 - t4;
    }
    last = need;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for the requested sequence */

    if (cache == NULL || n > last) {
        /* Allocate at least up to max(4, n). */
        idx_t need = (n > 4) ? n : 4;
        grow_cache(need);
    }
    return cache[(size_t)n];
}

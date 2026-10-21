/*
 * A223284  Rolling icosahedron face footprints: number of 3 X n 0..19 arrays
 *          starting with 0 where 0..19 label faces of an icosahedron and
 *          every array movement to a horizontal, diagonal or antidiagonal
 *          neighbor moves across an icosahedral edge.
 * https://oeis.org/A223284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 18*a(n-2) - 24*a(n-3) for n > 4 a(1) = 400,
 *            a(2) = 75, a(3) = 849, a(4) = 4995 (indices are 1‑based, a(0) is
 *            unused)
 * Range:     u8 none, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for 1 ≤ i ≤ size (cache[0] unused) */
    static seq_t *cache   = NULL;
    static size_t size    = 0;      /* highest index currently cached   */
    static size_t capacity = 0;     /* allocated size (≥ size+1)        */

    /* Make sure the cache can hold indices up to n (plus 0) */
    size_t needed = (size_t)n + 1;          /* indices 0 … n   */
    if (needed < 5)                         /* store first four terms anyway */
        needed = 5;
    if (needed > capacity) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();                  /* allocation failure */
        cache = tmp;
        capacity = needed;
    }

    /* Initialise the first four terms on first use */
    if (size == 0) {
        cache[0] = (seq_t)0;          /* unused placeholder      */
        cache[1] = (seq_t)400;
        cache[2] = (seq_t)75;
        cache[3] = (seq_t)849;
        cache[4] = (seq_t)4995;
        size = 4;
    }

    /* If the requested value is already cached, return it */
    if (n <= size) {
        return cache[n];
    }

    /* Compute missing values up to n */
    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t const a1 = cache[i - 1];
        seq_t const a2 = cache[i - 2];
        seq_t const a3 = cache[i - 3];
        seq_t term = a1 * (seq_t)5 + a2 * (seq_t)18 - a3 * (seq_t)24;
        cache[i] = term;
    }
    size = n;
    return cache[n];
}

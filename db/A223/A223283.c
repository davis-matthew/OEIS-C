/*
 * A223283  Rolling icosahedron face footprints: number of 2 X n 0..19 arrays
 *          starting with 0 where 0..19 label faces of an icosahedron and
 *          every array movement to a horizontal, diagonal or antidiagonal
 *          neighbor moves across an icosahedral edge.
 * https://oeis.org/A223283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-2) for n>3, a(1)=20, a(2)=15, a(3)=87
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed values.
       cache[i] stores a(i+1) (a(1) at cache[0]). */
    static seq_t *cache = NULL;
    static idx_t size = 0;

    if (n == 0) return (seq_t)0;   /* unused, but safe */

    /* Initialize the cache with the first three values if needed. */
    if (cache == NULL) {
        size = 3;
        cache = (seq_t*)malloc(size * sizeof(seq_t));
        cache[0] = (seq_t)20;   /* a(1) */
        cache[1] = (seq_t)15;   /* a(2) */
        cache[2] = (seq_t)87;   /* a(3) */
    }

    /* If the requested value is already cached, return it. */
    if (n <= size) return cache[n - 1];

    /* Extend the cache to accommodate up to n. */
    idx_t new_size = n;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) {
        /* Allocation failed; in a real program we might exit or handle it.
           For the purposes of this sequence function, we simply return 0. */
        return (seq_t)0;
    }
    cache = tmp;

    /* Compute new values using the recurrence. */
    for (idx_t i = size; i < n; ++i) {
        /* i is the zero‑based index for a(i+1). */
        seq_t val = cache[i - 1] * (seq_t)3 + cache[i - 2] * (seq_t)6;
        cache[i] = val;
    }

    size = new_size;
    return cache[n - 1];
}

/*
 * A223323  Rolling icosahedron footprints: number of 3 X n 0..11 arrays
 *          starting with 0 where 0..11 label vertices of an icosahedron and
 *          every array movement to a horizontal or antidiagonal neighbor
 *          moves along an icosahedral edge.
 * https://oeis.org/A223323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=144, a(2)=3125, a(3)=105625, a(4)=3570125,
 *            a(n)=38*a(n-1)-129*a(n-2) for n>4. (Offset 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute term a(n) of A223323, 1‑based index. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n<1, any value OK */

    /* Ensure the cache holds values up to n. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Minimum size we must allocate: at least 4 for the base cases,
       and at least n for any requested term. */
    size_t needed = (n > 4 ? (size_t)n : 4);

    /* First call: allocate and initialise the base cases. */
    if (cache == NULL)
    {
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;      /* allocation failed – undefined */
        cache[0] = (seq_t)144;      /* a(1) */
        cache[1] = (seq_t)3125;     /* a(2) */
        cache[2] = (seq_t)105625;   /* a(3) */
        cache[3] = (seq_t)3570125;  /* a(4) */
        cache_size = 4;
    }
    /* Grow the cache if a larger n is requested. */
    else if (cache_size < needed)
    {
        size_t old = (size_t)cache_size;
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;      /* reallocation failed */
        cache = tmp;
        /* Compute any new values using the recurrence. */
        for (size_t i = old; i < needed; ++i)
        {
            /* i refers to the index i in 0‑based array; it represents a(i+1). */
            cache[i] = 38 * cache[i - 1] - 129 * cache[i - 2];
        }
        cache_size = needed;
    }

    /* Return the requested term. */
    return cache[n - 1];
}

/*
 * A223693  Petersen graph (8,2) coloring a rectangular array: number of 2 X n
 *          0..15 arrays where 0..15 label nodes of a graph with edges 0,1 0,8
 *          8,14 8,10 1,2 1,9 9,15 9,11 2,3 2,10 10,12 3,4 3,11 11,13 4,5 4,12
 *          12,14 5,6 5,13 13,15 6,7 6,14 7,0 7,15 and every array movement to
 *          a horizontal or antidiagonal...
 * https://oeis.org/A223693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A223693: number of 2×n arrays of 0..15 labels
 * Recurrence:  a(n) = 8*a(n-1) - 11*a(n-2) - 16*a(n-3)   for n>4
 * Initial values: a(1)=256, a(2)=432, a(3)=2304, a(4)=12384
 * (Sequence is indexed starting at 1)
 */

#include <stdlib.h>   /* malloc, realloc, abort */

/* Static cache for memoised terms.
   a_cache[0] is unused; indices 1..a_cache_size-1 store terms. */
static seq_t *a_cache = NULL;   /* Pointer to allocated array of seq_t */
static idx_t  a_cache_size = 0; /* Current allocated size (index of last element+1) */

/* Ensure storage for indices up to n (inclusive) and initialise
 * base terms if the array is newly enlarged. */
static void ensure_cache(idx_t n)
{
    /* Already sufficient?  a_cache_size counts the number of slots allocated. */
    if (n + 1 <= a_cache_size)
        return;

    /* Allocate or resize the array */
    idx_t new_size = n + 1; /* include index 0 which is unused */
    seq_t *new_cache = (seq_t *)realloc(a_cache, (size_t)new_size * sizeof(seq_t));
    if (!new_cache) abort();            /* Out of memory */
    a_cache        = new_cache;
    a_cache_size   = new_size;

    /* Initialise base terms for the new region if necessary.
       Setting them again will not harm because the values are the same. */
    if (n >= 1) a_cache[1] = 256;
    if (n >= 2) a_cache[2] = 432;
    if (n >= 3) a_cache[3] = 2304;
    if (n >= 4) a_cache[4] = 12384;
}

/* Compute the n-th term of the sequence, with memoisation. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* sequence is undefined for n < 1 */

    /* Make sure we have storage for up to n */
    ensure_cache(n);

    /* Compute any missing terms up to n */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (i <= 4)
        {
            /* Base cases already initialised in ensure_cache */
            continue;
        }
        /* a(i) = 8*a(i-1) - 11*a(i-2) - 16*a(i-3) */
        seq_t term = (seq_t)8 * a_cache[i-1];
        term -= (seq_t)11 * a_cache[i-2];
        term -= (seq_t)16 * a_cache[i-3];
        a_cache[i] = term;
    }

    return a_cache[n];
}

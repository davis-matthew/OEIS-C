/*
 * A211272  Number of integer pairs (x,y) such that 0<x<=y<=n and
 *          x*y=floor(n/2).
 * https://oeis.org/A211272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(A000005(floor(n/2))/2) (A000005 = number of
 *            divisors)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Cache of divisor counts for indices 0 .. divcnt_cap-1 */
static seq_t *divcnt   = NULL;   /* divisor counts array */
static idx_t  divcnt_cap = 0;    /* current capacity */

/* Recompute divisor counts up to and including 'limit' (limit >=0). */
static void compute_divcnt_upto(idx_t limit)
{
    if (divcnt_cap > limit)
        return; /* already large enough */

    /* Grow capacity to the next power of two > limit. */
    idx_t new_cap = divcnt_cap ? divcnt_cap : 2;
    while (new_cap <= limit)
        new_cap <<= 1;

    seq_t *new_arr;
    if (divcnt_cap == 0)
        new_arr = seq_alloc(new_cap);                          /* first allocation */
    else
        new_arr = seq_realloc(divcnt, divcnt_cap, new_cap);     /* resize */

    if (!new_arr) abort();   /* allocation failure */

    /* Initialize all entries to zero.  */
    for (idx_t i = 0; i < new_cap; ++i)
        new_arr[i] = 0;

    /* Compute divisor counts for all indices up to new_cap-1. */
    for (idx_t d = 1; d < new_cap; ++d) {
        for (idx_t j = d; j < new_cap; j += d)
            new_arr[j] += 1;
    }

    /* Commit the new cache.  */
    divcnt     = new_arr;
    divcnt_cap = new_cap;
}

/* Return a(n) for n >= 1.  */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n = 0; return 0 */

    idx_t m = n / 2;           /* floor(n/2) */
    compute_divcnt_upto(m);    /* ensure divisor counts up to m are ready */

    seq_t d = divcnt[m];       /* number of divisors of m */
    return (d + 1) / 2;        /* ceil(d/2)  */
}

/*
 * A204998  a(n) = k^2 - j^2, where (k^2,j^2) is the least pair of distinct
 *          squares for which n divides their difference.
 * https://oeis.org/A204998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the difference of the least pair of distinct squares
 *            (k², j²) such that n divides k²−j², where the pair is chosen
 *            with the smallest possible k (i.e., the smallest larger square)
 * Range:     u8 n <= 30, u16 n <= 2198, u32 n <= 9934, u64 n <= 9934,
 *            u128 n <= 9934, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Allocate an array that stores the first occurrence of each residue
       modulo n.  Indices run from 0 to n‑1. */
    idx_t *first = (idx_t *)malloc(n * sizeof(idx_t));
    if (!first) return (seq_t)0;            /* safety fallback */
    memset(first, 0, n * sizeof(idx_t));

    for (idx_t k = 1; ; ++k) {
        seq_t k2 = (seq_t)k * (seq_t)k;        /* k² as seq_t */
        idx_t r = (idx_t)(k2 % (seq_t)n);      /* residue modulo n */

        idx_t j = first[r];                   /* previous k giving same residue */
        if (j != 0) {                         /* found a valid pair (k, j) */
            seq_t j2 = (seq_t)j * (seq_t)j;
            seq_t diff = k2 - j2;             /* k² − j²  */
            free(first);
            return diff;
        }
        first[r] = k;                         /* remember this occurrence */
    }
}

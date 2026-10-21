/*
 * A205003  The index j<k such that n divides s(k)-s(j), where k is the least
 *          index (A205002) for which such j exists, and s(k)=k(k+1)/2.
 * https://oeis.org/A205003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A205003:  a(n) is the index j<k such that n divides s(k)-s(j),
 *  where k is the smallest index for which such a j exists and s(k)=k(k+1)/2.
 *
 *  Algorithm:  compute triangular numbers modulo n in increasing order.
 *  Keep the first index at which each residue appears in an array.
 *  The first time a residue repeats we have found the minimal k;
 *  the stored index is the required j.  Complexity O(n).
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* n is 1‑based:  n>=1; return 0 if memory allocation fails. */
    if (n == 0) return 0;

    /* Allocate an array of size n for residues 0..n-1. */
    idx_t *first = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!first) return 0;

    memset(first, 0, (size_t)n * sizeof(idx_t));

    for (idx_t k = 1; ; ++k) {
        /* Compute triangular number modulo n using 128‑bit arithmetic
           to avoid overflow on k up to 2^64. */
        __int128 t = (__int128)k * (k + 1) / 2;
        idx_t r = (idx_t)(t % n);

        if (first[r] == 0) {
            /* First time this residue occurs.  Store the index. */
            first[r] = k;
        } else {
            /* Repeat found – minimal k achieved.  Return the earlier index. */
            seq_t result = first[r];
            free(first);
            return result;
        }
    }
}

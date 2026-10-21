/*
 * A204997  The square j^2 such that n divides k^2-j^2>0, where k is the least
 *          positive integer for which such a j exists.
 * https://oeis.org/A204997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A204997.
   For a given n, find the least positive integer k such that
   there exists j < k with n | (k² – j²) > 0.  The function returns
   the square j² corresponding to that minimal k. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */

    /* first[r] stores the first k that produced residue r = k² mod n.
       0 means “not seen yet”.  j cannot be 0 since we start with k=1. */
    idx_t *first = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!first) return (seq_t)0;          /* allocation failure */

    for (idx_t k = 1; ; ++k) {
        /* compute k² mod n safely using 128‑bit arithmetic when needed */
        idx_t r = (idx_t)(((unsigned __int128)k * k) % n);

        idx_t j = first[r];
        if (j) {                          /* residue seen before */
            seq_t ans = (seq_t)j * (seq_t)j;
            free(first);
            return ans;
        }

        first[r] = k;                     /* remember first appearance */
    }
}

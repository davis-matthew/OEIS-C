/*
 * A205028  The index j<k such that n divides s(k)-s(j), where k is the least
 *          index (A205018) for which such j exists, and s(k)=k*(k+1).
 * https://oeis.org/A205028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the index j < k such that n divides s(k)-s(j),
   where k is the smallest index for which such a j exists and
   s(t) = t*(t+1).  The function computes this definition directly. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1.  Return 0 for n = 0. */
    if (n == 0)
        return 0;

    /* Allocate an array to record the first occurrence of each
       residue of s(k) modulo n.  idx_t is sufficiently large
       to hold the residues because residues are < n. */
    idx_t *first = (idx_t*)calloc((size_t)n, sizeof(idx_t));
    if (!first)
        return 0;           /* allocation failure (unlikely for usual n) */

    for (idx_t k = 1; ; ++k) {
        /* Compute s(k) = k*(k+1) in seq_t, then its residue modulo n. */
        seq_t s = (seq_t)k * ((seq_t)k + 1);
        idx_t rem = (idx_t)(s % (seq_t)n);

        if (first[rem] == 0) {
            /* First time this residue appears; remember k. */
            first[rem] = k;
        } else {
            /* Residue repeats; previous occurrence is a(n). */
            seq_t result = (seq_t)first[rem];
            free(first);
            return result;
        }
    }
}

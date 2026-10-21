/*
 * A205395  The index j<k such that n divides s(k)-s(j) for some j, where
 *          s(j)=ceiling(j^2/2), and k is the least index for which such an
 *          index j exists.
 * https://oeis.org/A205395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the first index j (<k) such that the difference
   s(k)-s(j) is divisible by n, where s(i)=⌈i²/2⌉ and k is the least
   index with such a j.  Equivalently, a(n) equals the earliest index
   of a repeating residue in the sequence s(i) modulo n.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n=0, return 0 safely */

    size_t cap = (size_t)n;        /* array size for residues 0..n-1 */
    idx_t *first = (idx_t*)calloc(cap, sizeof(idx_t));
    if (first == NULL)
        return 0;                 /* allocation failure, unlikely for small n */

    for (idx_t i = 1;; ++i) {
        /* s(i) = ceil(i^2/2) = (i*i + 1)/2  */
        seq_t si = ((seq_t)i * (seq_t)i + 1) / 2;
        idx_t r = (idx_t)(si % (seq_t)n);

        if (first[r] == 0) {
            first[r] = i;         /* first occurrence of this residue */
        } else {
            seq_t result = (seq_t)first[r];
            free(first);
            return result;        /* j found, return as seq_t */
        }
    }
}

/*
 * A205018  Least k such that n divides s(k)-s(j) for some j satisfying
 *          1<=j<k, where s(j)=j*(j+1).
 * https://oeis.org/A205018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>j≥1 such that n | k(k+1)−j(j+1); equivalently the
 *            first k where m(m+1) mod n repeats for m≥1
 * Range:     u8 n <= 30, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)    /* undefined, but guard against bad input */
        return 0;

    /* visited residues modulo n */
    bool *seen = (bool *)calloc((size_t)n, sizeof(bool));
    if (!seen)    /* allocation failure */
        return 0;

    /* iterate starting from m = 1 */
    for (idx_t m = 1;; ++m) {
        seq_t val = (seq_t)m * ((seq_t)m + 1);   /* m(m+1) in seq_t */
        idx_t r = (idx_t)(val % (seq_t)n);       /* residue modulo n */

        if (seen[r]) {                           /* first repeat found */
            free(seen);
            return (seq_t)m;                     /* minimal k = m */
        }
        seen[r] = true;                          /* record this residue */
    }
}

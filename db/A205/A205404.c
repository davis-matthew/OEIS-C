/*
 * A205404  Least s(k) such that n divides s(k)-s(j) for some j < k, where
 *          s(j) = floor((j+1)^2/2)/2.
 * https://oeis.org/A205404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least s(k) such that n | s(k)-s(j) for some j<k, * where
 *            s(j) = floor((j+1)^2/4) (since floor(floor((j+1)^2/2)/2) *
 *            simplifies to floor(((j+1)^2)/4)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* be tolerant of n==0; the offset of the sequence is 1 */
    if (n == (idx_t)0) return (seq_t)0;

    /* Allocate a visited–residue array of size n */
    uint8_t *seen = (uint8_t*)calloc((size_t)n, 1);
    if (!seen) return (seq_t)0;     /* allocation failure fallback */

    /* initialize with the first value s(1) */
    {
        seq_t s1 = ((seq_t)2 * (seq_t)2) / 4;         /* (1+1)^2 = 4 */
        idx_t r1 = (idx_t)(s1 % (seq_t)n);
        seen[r1] = 1;
    }

    /* iterate k = 2,3,... until a residue repeats */
    for (idx_t k = 2;; ++k) {
        seq_t sk = ((seq_t)(k + 1) * (seq_t)(k + 1)) / 4;
        idx_t rk = (idx_t)(sk % (seq_t)n);
        if (seen[rk]) {
            free(seen);
            return sk;
        }
        seen[rk] = 1;
    }
}

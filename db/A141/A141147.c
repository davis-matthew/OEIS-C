/*
 * A141147  Number of linear arrangements of n blue, n red and n green items
 *          such that the first item is blue and there are no adjacent items
 *          of the same color (first and last elements considered as
 *          adjacent).
 * https://oeis.org/A141147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} C(n,2k) * C(2k,k) * C(n-1+k,k) *
 *            2^(n-2k) (OEIS A141147)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1, but safe */
    seq_t result = 0;
    idx_t half = n >> 1;          /* floor(n/2) */
    for (idx_t k = 0; k <= half; ++k) {
        seq_t t1 = binom(n, 2*k);                 /* C(n,2k) */
        seq_t t2 = binom(2*k, k);                 /* C(2k,k) */
        seq_t t3 = binom(n - 1 + k, k);           /* C(n-1+k,k) */
        seq_t t4 = pow2(n - 2*k);                 /* 2^(n-2k) */
        seq_t prod = t1;
        prod *= t2;
        prod *= t3;
        prod *= t4;
        result += prod;
    }
    return result;
}

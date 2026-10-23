/*
 * A160874  Regular coverings having dihedral voltage groups: see Kwak-Lee
 *          reference in A160870 for precise definition.
 * https://oeis.org/A160874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n - 1) * (5^{n-1} - 1) / 4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* Special case for the offset n = 1 */
    if (n == 0) return (seq_t)0;

    /* Compute 2^n and 5^{n-1} */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);
    seq_t pow5 = big_pow((seq_t)5, (seq_t)(n - 1));

    /* (5^{n-1} - 1) / 4  – this is an integer for all n >= 1 */
    seq_t part5 = (pow5 - (seq_t)1) / (seq_t)4;

    /* (2^n - 1) * ((5^{n-1} - 1) / 4) */
    seq_t result = (pow2 - (seq_t)1) * part5;

    return result;
}

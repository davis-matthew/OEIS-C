/*
 * A147540  Numbers whose binary representation is the concatenation of 2n-1
 *          digits 1, n digits 0 and 2n-1 digits 1.
 * https://oeis.org/A147540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 2^(5*n-2) - 2^(3*n-1) + 2^(2*n-1) - 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Shifts are safe because n is >= 1 and the tests only
       request values that fit in seq_t, so the shift counts
       are less than the bit width of seq_t. */
    idx_t e1 = 5*n - 2;   /* exponent for 2^(5n-2) */
    idx_t e2 = 3*n - 1;   /* exponent for 2^(3n-1) */
    idx_t e3 = 2*n - 1;   /* exponent for 2^(2n-1) */

    seq_t term1 = ((seq_t)1 << e1);
    seq_t term2 = ((seq_t)1 << e2);
    seq_t term3 = ((seq_t)1 << e3);

    return term1 - term2 + term3 - 1;
}

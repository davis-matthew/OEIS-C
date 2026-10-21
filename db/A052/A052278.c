/*
 * A052278  a(n) = (4n+3)!/4^n.
 * https://oeis.org/A052278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n+3)! / 4^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    // Compute (4*n+3)!
    seq_t up = ((seq_t)4 * (seq_t)n) + (seq_t)3;
    for (seq_t i = 1; i <= up; ++i)
        result *= i;
    // Compute 4^n
    seq_t pow4 = 1;
    for (idx_t k = 0; k < n; ++k)
        pow4 *= (seq_t)4;
    // Divide by 4^n
    result /= pow4;
    return result;
}

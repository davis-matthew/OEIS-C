/*
 * A096024  Numbers n such that (n+j) mod (2+j) = 1 for j from 0 to 5 and
 *          (n+6) mod 8 <> 1.
 * https://oeis.org/A096024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 840*n - 417
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* valid for n >= 1; undefined for n == 0 */
    seq_t result = (seq_t)840 * (seq_t)n - (seq_t)417;
    return result;
}

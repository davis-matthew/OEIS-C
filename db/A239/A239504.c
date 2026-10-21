/*
 * A239504  Number of digits in the decimal expansion of n^10 (A008454).
 * https://oeis.org/A239504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(10*log10(n))+1 for n>0, a(0)=1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   digit_len, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t p = pow_uint((uint64_t)n, 10);
    return (seq_t)digit_len(p);
}

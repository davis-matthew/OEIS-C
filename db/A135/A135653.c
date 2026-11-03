/*
 * A135653  Divisors of 496 (the 3rd perfect number), written in base 2.
 * https://oeis.org/A135653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full, easy, less
 * Formula:   a(n) = the decimal number obtained by interpreting the binary
 *            representation of the nth divisor of 496 as a decimal integer
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: full, pass, 10 terms through n = 10 (2026-11-02)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bit_length.h"

/* nth divisor of 496:
   for n=1..5  -> 1 << (n-1)
   for n=6..10 -> 31 << (n-6)  */
static seq_t nth_divisor_of_496(idx_t n)
{
    if (n <= 5)
        return ((seq_t)1) << (unsigned)(n - 1);
    else
        return ((seq_t)31) << (unsigned)(n - 6);
}

seq_t a(idx_t n)
{
    /* Only defined for 1 ≤ n ≤ 10 */
    if (n < 1 || n > 10)
        return 0;

    seq_t d = nth_divisor_of_496(n);

    /* Convert the binary representation of d into a decimal number
       whose digits are the bits of d, most significant bit first */
    seq_t result = 0;
    idx_t msb = (idx_t)bit_length(d);   /* number of bits in d */
    for (idx_t i = msb; i > 0; --i) {
        seq_t bit = (d >> (i - 1)) & ((seq_t)1);
        result = result * ((seq_t)10) + bit;
    }
    return result;
}

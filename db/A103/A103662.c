/*
 * A103662  Smallest power with base>1 and exponent n whose decimal
 *          representation doesn't contain the digit 0.
 * https://oeis.org/A103662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest b^n with b>1 whose decimal representation *
 *            contains no digit 0; a(0)=1. If no such b exists for a given n,
 *            * this implementation returns 1
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 16, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   digit_mask, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_mask.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    for (idx_t b = 2; ; ++b) {
        seq_t val = pow_seq((seq_t)b, n);
        /* digit_mask returns a bitmask of digits 0..9 present in val.
           Bit 0 corresponds to digit 0.  */
        if ((digit_mask(val) & 1) == 0)
            return val;
    }
}

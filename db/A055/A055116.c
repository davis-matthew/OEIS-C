/*
 * A055116  Base-6 complement of n (write n in base 6, then replace each digit
 *          with its base-6 negative).
 * https://oeis.org/A055116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = digitwise base-6 complement of n: each base‑6 digit d is
 *            replaced by (6-d) % 6
 * Range:     u8 n <= 215, u16 n <= 7775, u32 n <= 7775, u64 n <= 7775,
 *            u128 n <= 7775, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;
    while (n != 0)
    {
        uint64_t digit = n % 6;
        seq_t comp = (seq_t)((6 - digit) % 6);   // complement of the digit
        result += comp * power;
        power *= (seq_t)6;
        n /= 6;
    }
    return result;
}

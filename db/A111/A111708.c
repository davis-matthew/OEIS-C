/*
 * A111708  a(n) = n concatenated with 9's complement of n.
 * https://oeis.org/A111708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (n + 1) * (10^k - 1), where k is the decimal digit count
 *            of n
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   count_digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_digits.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Determine the number of decimal digits of n. */
    int k = count_digits(n);          /* count_digits(0) returns 1 */

    /* Compute 10^k - 1 */
    seq_t ten_pow = pow10((idx_t)k);
    seq_t mult    = ten_pow - 1;

    /* a(n) = (n + 1) * (10^k - 1) */
    return ((seq_t)n + 1) * mult;
}

/*
 * A173525  a(n) = 1 + A053824(n-1), where A053824 = sum of digits in base 5.
 * https://oeis.org/A173525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = 1 + sum of base-5 digits of (n-1)
 * Range:     u8 n <= 3126, u16 n <= 3126, u32 n <= 3126, u64 n <= 3126,
 *            u128 n <= 3126, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for standard integer types, harmless */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    idx_t m = n - 1;             /* value whose base-5 digits we sum */
    seq_t sum = (seq_t)0;        /* sum of the digits, fits in seq_t */
    while (m > 0) {
        sum += (seq_t)(m % 5);   /* add least significant base-5 digit */
        m /= 5;                  /* shift right by one base-5 digit */
    }
    return sum + (seq_t)1;       /* a(n) = 1 + sum of digits */
}

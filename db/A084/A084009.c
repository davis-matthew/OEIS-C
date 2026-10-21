/*
 * A084009  a(n) = n^2 concatenated with reverse(n^2) divided by 11.
 * https://oeis.org/A084009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (n^2 concatenated with reverse(n^2))/11
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow10_of_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10_of_digits.h"
/* Implementation using only integer arithmetic on seq_t and idx_t. */

static seq_t reverse_decimal(seq_t x)
{
    seq_t rev = 0;
    while (x) {
        rev = rev * 10 + (x % 10);
        x = x / 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* cast index to seq_t */
    seq_t m  = nn * nn;             /* n^2 */
    seq_t rev = reverse_decimal(m); /* reverse of m */

    seq_t pow10 = pow10_of_digits(m);         /* 10^digits(m) */
    seq_t concat = m * pow10 + rev;           /* concatenate m and rev(m) */
    seq_t result = concat / 11;               /* divide by 11 */

    return result;
}

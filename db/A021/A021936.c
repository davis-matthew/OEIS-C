/*
 * A021936  Decimal expansion of 1/932.
 * https://oeis.org/A021936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the (n+1)-th decimal digit of 1/932, starting with a(0) =
 *            the first digit after the decimal point. The sequence repeats
 *            with period equal to the multiplicative order of 10 modulo 932.
 *            We compute it by long division: r = remainder, digit = r/932
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The remainder after dividing 1 by 932.  It always stays < 932. */
    idx_t r = 1;
    /* Compute the (n+1)-th digit: perform long division n+1 times. */
    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;
        seq_t digit = (seq_t)(r / 932);
        r %= 932;
        if (i == n) {
            return digit;
        }
    }
    /* Unreachable, but avoid missing return. */
    return 0;
}

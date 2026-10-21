/*
 * A077311  a(n) = A077310(n)/n.
 * https://oeis.org/A077311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (concatenation of consecutive integers from n+1 up to the
 *            smallest m such that the resulting number is divisible by n) /
 *            n. The concatenation is formed by writing the consecutive
 *            integers in decimal without separators. The function returns
 *            that quotient
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    idx_t next = n + 1;            /* first integer to concatenate */
    seq_t concat = 0;              /* concatenated value as seq_t   */
    idx_t rem = 0;                 /* remainder of concatenated value mod n */

    while (1) {
        int d = digits(next);      /* number of decimal digits of next */

        /* Compute 10^d as seq_t and as remainder modulo n. */
        seq_t pow10 = 1;
        idx_t pow10_mod = 1 % n;   /* safe even for n == 1 */
        for (int i = 0; i < d; ++i) {
            pow10 *= 10;           /* exact multiplication in seq_t   */
            pow10_mod = (pow10_mod * 10) % n;
        }

        /* Update remainder before modifying concat. */
        rem = (rem * pow10_mod + next) % n;

        /* Append next to the concatenated value. */
        concat = concat * pow10 + (seq_t)next;

        if (rem == 0) {
            break;
        }
        ++next;
    }

    return concat / (seq_t)n;
}

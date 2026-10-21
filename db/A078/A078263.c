/*
 * A078263  Product of the forward and reverse concatenations of 1 to n.
 * https://oeis.org/A078263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A000422(n) * A007908(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   digits_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digits_of.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* values for n < 1 are undefined */

    seq_t f = 0;      /* forward concatenation */
    seq_t r = 0;      /* reverse concatenation */

    /* build forward concatenation 1..n */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t di = digits_of(i);
        f = f * pow10(di) + i;
    }

    /* build reverse concatenation by appending numbers from n down to 1 */
    for (idx_t i = n; i > 0; --i) {
        idx_t di = digits_of(i);
        r = r * pow10(di) + i;
    }

    return f * r;
}

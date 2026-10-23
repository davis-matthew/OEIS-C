/*
 * A028440  Golc sequence in base 10. Left to right concatenation of
 *          n,int(log_10(n)),int(log_10(int(log_10(n)))),... in base 10.
 * https://oeis.org/A028440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concat(n, digits(n) - 1) (in base 10)
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    /* number of decimal digits of n */
    idx_t d = (idx_t)digits(n);
    /* value to append: digits(n)-1 */
    idx_t append = d - 1;

    /* width of the appended value in decimal digits */
    idx_t width = (append == 0) ? 1 : (idx_t)digits(append);

    /* concatenate n followed by the appended value */
    seq_t result = (seq_t)n;
    result = result * pow10(width) + (seq_t)append;
    return result;
}

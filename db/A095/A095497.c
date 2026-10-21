/*
 * A095497  a(n) = 52 written in base 13 - n.
 * https://oeis.org/A095497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal integer obtained by writing the decimal number 52
 *            in base (13 - n). For n = 12 the base is 1, giving a unary
 *            representation (52 ones). For n ≥ 13 the base is 0, which is
 *            undefined; the function returns 0 in this case
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The base in which to write 52.  */
    int64_t base = 13 - (int64_t)n;

    /* Handle undefined base or very small bases. */
    if (base == 0)            /* base == 0 */ return 0;
    if (base == 1) {          /* unary representation */
        /* 52 ones.  We know 52 fits in uint64_t. */
        seq_t result = 0;
        for (uint64_t i = 0; i < 52; ++i)
            result = result * 10 + 1;
        return result;
    }

    /* For any other base (positive or negative) compute the
       representation of the fixed decimal 52.  */
    const seq_t VALUE = (seq_t)52;

    /* Store digits (least significant first).  */
    uint8_t digits[200];
    int dpos = 0;

    int64_t temp = (int64_t)VALUE;
    int64_t b = base;            /* signed base. */

    /* Special case: value 0.  */
    if (temp == 0) {
        return 0;
    }

    /* Convert using Euclidean division. */
    while (temp != 0) {
        int64_t q = temp / b;
        int64_t r = temp - q * b;     /* Equivalent to temp % b but
                                         always gives the remainder with
                                         the same sign as temp. */
        if (r < 0) {
            r += (b < 0 ? -b : b);
            q += 1;
        }
        digits[dpos++] = (uint8_t)r;
        temp = q;
    }

    /* Build the decimal integer from the digits. */
    seq_t result = 0;
    for (int i = dpos - 1; i >= 0; --i) {
        result = result * 10 + digits[i];
    }
    return result;
}

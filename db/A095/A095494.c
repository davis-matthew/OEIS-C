/*
 * A095494  a(n) = 51 written in base n.
 * https://oeis.org/A095494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the decimal number obtained by writing 51 in base n *
 *            (i.e., write 51 in base n, then concatenate the base‑n digits as
 *            * decimal digits)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   dec_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/dec_len.h"

seq_t a(idx_t n) {
    /* Handle unary base n == 1 : 51 ones (unary representation). */
    if (n == 1) {
        seq_t res = 0;
        for (int i = 0; i < 51; ++i) {
            res = res * (seq_t)10 + (seq_t)1;
        }
        return res;
    }

    /* Convert the integer 51 to base n.  Since 51 fits in an int, work
     * with an int for the conversion.  The base n may be large, but
     * the number of digits is at most 6 (for base 2) and at most 51
     * for base 1 which we handled above. */
    int v = 51;
    int digits[10];          /* large enough for any n >= 2 (max 6 digits). */
    int sz = 0;
    while (v > 0) {
        digits[sz++] = v % (int)n;
        v /= (int)n;
    }

    /* Build the concatenated decimal number from most significant digit
     * to least.  For digits >= 10, the decimal representation is 2 or
     * more decimal digits.  We multiply by 10^len(d) each step. */
    seq_t result = 0;
    for (int i = sz - 1; i >= 0; --i) {
        int d = digits[i];
        int len = dec_len(d);
        seq_t mult = 1;
        for (int k = 0; k < len; ++k)
            mult *= (seq_t)10;
        result = result * mult + (seq_t)d;
    }
    return result;
}

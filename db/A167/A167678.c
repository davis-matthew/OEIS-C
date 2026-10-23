/*
 * A167678  Replace prime digits with 2 and nonprime digits with 1.
 * https://oeis.org/A167678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   For n, replace every decimal digit * d by 2 if d∈{2,3,5,7} else
 *            by 1, and interpret the resulting * decimal string as the value
 *            a(n)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* mapping of decimal digits to replaced digit */
    static const unsigned char map[10] = {
        1, 1, 2, 2, 1, 2, 1, 2, 1, 1
    };

    /* store the digits of n in reverse order */
    unsigned char digits[20];
    size_t len = 0;
    idx_t m = n;

    if (m == 0) {
        digits[len++] = 0;
    } else {
        while (m > 0) {
            digits[len++] = (unsigned char)(m % 10);
            m /= 10;
        }
    }

    /* build the result from most significant to least */
    seq_t res = 0;
    int i;
    for (i = (int)len - 1; i >= 0; --i) {
        unsigned char d = digits[i];
        res = res * 10 + map[d];
    }

    return res;
}

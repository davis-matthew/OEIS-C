/*
 * A075010  a(n) = floor( concatenation of 9 numbers from n+8 to n in that
 *          order divided by 9 ).
 * https://oeis.org/A075010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor( concat of numbers (n+8),(n+7),…,(n) / 9 )
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t con = 0;
    for (idx_t i = 0; i < 9; ++i) {
        /* current number in the concatenation */
        idx_t cur = n + 8 - i;

        /* count decimal digits of cur (handle cur==0 correctly) */
        idx_t tmp = cur;
        int d = 0;
        if (tmp == 0) {
            d = 1;
        } else {
            while (tmp) {
                ++d;
                tmp /= 10;
            }
        }

        /* concatenate: con = con * 10^d + cur */
        con = con * pow10((idx_t)d) + (seq_t)cur;
    }
    return con / (seq_t)9;   /* floor division by 9 */
}

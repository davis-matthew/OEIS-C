/*
 * A018299  Divisors of 136.
 * https://oeis.org/A018299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 136 in increasing order
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divs[8];
    static size_t cnt = 0;

    if (cnt == 0) {
        /* generate divisors 2^a * 17^b with a=0..3, b=0..1 */
        size_t i = 0;
        for (int a = 0; a <= 3; ++a) {
            seq_t pow2 = ((seq_t)1) << a;      /* 1,2,4,8 */
            for (int b = 0; b <= 1; ++b) {
                seq_t d = pow2 * ((b == 0) ? (seq_t)1 : (seq_t)17);
                divs[i++] = d;
            }
        }
        /* sort ascending */
        for (size_t p = 0; p < 8; ++p) {
            for (size_t q = p + 1; q < 8; ++q) {
                if (divs[q] < divs[p]) {
                    seq_t tmp = divs[p];
                    divs[p] = divs[q];
                    divs[q] = tmp;
                }
            }
        }
        cnt = 8;
    }

    if (n >= 1 && n <= cnt)
        return divs[(size_t)(n - 1)];
    return (seq_t)0;
}

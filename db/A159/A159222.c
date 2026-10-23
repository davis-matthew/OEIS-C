/*
 * A159222  1/4 the number of n X n arrays of squares of integers with every
 *          2X2 subblock summing to 21.
 * https://oeis.org/A159222
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - (2^⌊(n-6)/2⌋ + 2 - 7) for n >= 8 with a(2)=7,
 *            a(3)=20, a(4)=45, a(5)=95, a(6)=193, a(7)=389
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                    /* undefined for n < 2 */

    /* known values for 2 <= n <= 7 */
    static const seq_t base[8] = {0, 0, 7, 20, 45, 95, 193, 389};

    if (n < 8) return base[n];

    seq_t val = base[7];                     /* a(7) */
    seq_t pow2 = pow_uint(2, 3);             /* 2^3 for i = 8 */

    for (idx_t i = 8; i <= n; ++i) {
        /* double pow2 when |i-6| changes parity (i.e., at even i >= 10) */
        if (i > 8 && ((i - 6) & 1) == 0) {
            pow2 += pow2;                    /* multiply by 2 */
        }

        seq_t diff = pow2;
        diff -= 7;                            /* 2^(k+2) - 7 */

        val += val;                           /* 2 * val */
        val -= diff;                          /* val - diff */
    }

    return val;
}

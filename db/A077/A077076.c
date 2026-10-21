/*
 * A077076  Trajectory of 537 under the Reverse and Add! operation carried out
 *          in base 2, written in base 10.
 * https://oeis.org/A077076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 537, a(1) = 1146, a(2) = 1899, a(3) = 3618, a(4) = 4713,
 *            a(5) = 9522, a(6) = 14427, a(7) = 28386, a(8) = 37533, a(9) =
 *            84966, a(10) = 138123, a(11) = 353004; For n > 11: if n = 0 (mod
 *            4): a(n) = 3*2^(2*k+13)+18249*2^k-3, k = (n-4)/4 if n = 1 (mod
 *            4): a(n) = 6*2^(2*k+13)-12102*2^k, k = (n-5...
 * Range:     u8 none, u16 n <= 8, u32 n <= 38, u64 n <= 102, u128 n <= 230,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t init[12] = {
        537, 1146, 1899, 3618, 4713,
        9522, 14427, 28386, 37533, 84966,
        138123, 353004
    };

    if (n < 12)
        return init[(idx_t)n];

    idx_t r = n & 3;               /* n % 4 */
    idx_t k;
    seq_t val;

    switch (r) {
    case 0: /* n = 0 mod 4 */
        k = (n - 4) >> 2;          /* (n-4)/4 */
        val = 3 * (((seq_t)1) << (2 * k + 13))
              + 18249 * (((seq_t)1) << k)
              - 3;
        break;
    case 1: /* n = 1 mod 4 */
        k = (n - 5) >> 2;          /* (n-5)/4 */
        val = 6 * (((seq_t)1) << (2 * k + 13))
              - 12102 * (((seq_t)1) << k);
        break;
    case 2: /* n = 2 mod 4 */
        k = (n - 6) >> 2;          /* (n-6)/4 */
        val = 6 * (((seq_t)1) << (2 * k + 13))
              + 11718 * (((seq_t)1) << k)
              - 3;
        break;
    case 3: /* n = 3 mod 4 */
        k = (n - 7) >> 2;          /* (n-7)/4 */
        val = 12 * (((seq_t)1) << (2 * k + 13))
              - 11910 * (((seq_t)1) << k);
        break;
    }

    return val;
}

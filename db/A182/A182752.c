/*
 * A182752  a(1) = 1, a(2) = 6, for n >= 3; a(n) = the smallest number greater
 *          than a(n-1) such that [[a(n-2) + a(n-1)] * [a(n-2) + a(n)] *
 *          [a(n-1) + a(n)]] / [a(n-2) * a(n-1) * a(n)] is an integer.
 * https://oeis.org/A182752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14^((n-1)/2) if n is odd, a(n) = 6 * 14^((n/2)-1) if n is
 *            even
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   power14
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power14.h"

extern seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, return 0 for safety */
    if (n == 1) return 1;
    if (n == 2) return 6;

    if (n & 1) {                   /* odd n = 2k+1 */
        idx_t k = (n - 1) >> 1;    /* k = (n-1)/2 */
        return power14(k);
    } else {                        /* even n = 2k */
        idx_t k = n >> 1;          /* k = n/2 */
        return 6 * power14(k - 1);
    }
}

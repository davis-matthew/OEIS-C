/*
 * A025037  Number of partitions of { 1, 2, ..., 5n } into sets of size 5.
 * https://oeis.org/A025037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5n)!/(n! (5!)^n)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* a(n) = (5n)!/(n! (5!)^n) */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t res = (seq_t)1;

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* numerator factors: 5i, 5i-1, ..., 5i-4 */
        seq_t nums[5];
        seq_t five_i = (seq_t)i * (seq_t)5;
        nums[0] = five_i;
        nums[1] = five_i - (seq_t)1;
        nums[2] = nums[1] - (seq_t)1;
        nums[3] = nums[2] - (seq_t)1;
        nums[4] = nums[3] - (seq_t)1;

        /* denominator factors: i, 2, 2, 2, 3, 5 */
        idx_t dens[6] = { i, (idx_t)2, (idx_t)2, (idx_t)2, (idx_t)3, (idx_t)5 };

        /* Cancel numerator factors against denominators */
        for (int ni = 0; ni < 5; ++ni) {
            seq_t num = nums[ni];
            for (int di = 0; di < 6; ++di) {
                if (dens[di] == (idx_t)1) {
                    continue;
                }
                seq_t g = gcd(num, (seq_t)dens[di]);
                if (g == (seq_t)1) {
                    continue;
                }
                num /= g;
                dens[di] /= (idx_t)g;
                if (num == (seq_t)1) {
                    break;
                }
            }
            /* multiply the remaining part of the numerator */
            res *= num;
        }

        /* Divide any remaining denominator factors by the result */
        for (int di = 0; di < 6; ++di) {
            while (dens[di] > (idx_t)1) {
                seq_t d_val = (seq_t)dens[di];
                seq_t g = gcd(res, d_val);
                if (g == (seq_t)1) {
                    break;
                }
                res /= g;
                dens[di] /= (idx_t)g;
            }
        }
    }

    return res;
}

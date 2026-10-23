/*
 * A010991  Binomial coefficient C(n,38).
 * https://oeis.org/A010991
 *
 * Offset:    38    (a(38) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,38) (binomial coefficient)
 * Range:     u8 n <= 39, u16 n <= 41, u32 n <= 47, u64 n <= 68, u128 n <= 174,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 38) {
        return 0;
    }

    const idx_t k = 38;
    seq_t nums[k];

    /* numerator factors: n, n-1, …, n-37 */
    for (idx_t i = 0; i < k; ++i) {
        nums[i] = (seq_t)(n - i);
    }

    /* cancel denominator factors 1..38 with numerators */
    for (idx_t d = 1; d <= k; ++d) {
        seq_t dval = (seq_t)d;
        for (idx_t j = 0; j < k && dval > 1; ++j) {
            seq_t g = gcd(nums[j], dval);
            if (g > 1) {
                nums[j] /= g;
                dval /= g;
            }
        }
    }

    /* multiply remaining numerator factors */
    seq_t res = 1;
    for (idx_t i = 0; i < k; ++i) {
        res *= nums[i];
    }
    return res;
}

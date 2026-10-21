/*
 * A215549  Number of standard Young tableaux of shape [9n,9].
 * https://oeis.org/A215549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(9*n+9,9) * (9*n-8) / (9*n+1) (a(0)=0)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 5, u64 n <= 63, u128 n <= 1000,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Build numerator terms:
       9*n+9, 9*n+8, ..., 9*n+1, 9*n-8   (10 terms) */
    seq_t nums[10];
    idx_t base = 9 * n + 9;
    for (idx_t i = 0; i < 9; ++i)
        nums[i] = (seq_t)(base - i);               /* 9*n+9 down to 9*n+1 */
    nums[9] = (seq_t)(9 * n - 8);                   /* extra multiplier */

    /* Build denominator terms:
       1, 2, ..., 9, 9*n+1   (10 terms) */
    seq_t dens[10];
    for (idx_t i = 0; i < 9; ++i)
        dens[i] = (seq_t)(i + 1);                   /* 1..9 */
    dens[9] = (seq_t)(9 * n + 1);                 /* final denominator */

    /* Reduce each denominator against the numerators */
    for (idx_t d = 0; d < 10; ++d) {
        seq_t denom = dens[d];
        if (denom <= 1)
            continue;
        for (idx_t j = 0; j < 10 && denom > 1; ++j) {
            seq_t g = seq_gcd(nums[j], denom);
            if (g > 1) {
                nums[j] /= g;
                denom /= g;
            }
        }
        dens[d] = denom;   /* must be 1 after full reduction */
    }

    /* Multiply the remaining numerators */
    seq_t result = 1;
    for (idx_t i = 0; i < 10; ++i)
        result *= nums[i];

    return result;
}

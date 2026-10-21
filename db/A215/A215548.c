/*
 * A215548  Number of standard Young tableaux of shape [8n,8].
 * https://oeis.org/A215548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(8*n+8,8)*(8*n-7)/(8*n+1), a(0)=0
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 7, u64 n <= 120, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(0) is defined to be 0 */
    if (n == 0)
        return (seq_t)0;

    /* base = 8*n */
    seq_t base = (seq_t)8 * n;

    /* Numerator terms: (8n+8), (8n+7), …, (8n+1), (8n-7)  */
    seq_t nums[9];
    nums[0] = base + (seq_t)8;
    nums[1] = base + (seq_t)7;
    nums[2] = base + (seq_t)6;
    nums[3] = base + (seq_t)5;
    nums[4] = base + (seq_t)4;
    nums[5] = base + (seq_t)3;
    nums[6] = base + (seq_t)2;
    nums[7] = base + (seq_t)1;
    nums[8] = base - (seq_t)7;           /* extra factor (8n-7) */

    /* Cancel the factorial part of the binomial coefficient
       (denominators 2 … 8). */
    for (seq_t d = (seq_t)2; d <= (seq_t)8; ++d) {
        seq_t t = d;
        for (size_t i = 0; i < 9 && t > (seq_t)1; ++i) {
            seq_t g = gcd(nums[i], t);
            if (g > (seq_t)1) {
                nums[i] /= g;
                t      /= g;
            }
        }
    }

    /* Cancel the extra denominator (8n+1). */
    seq_t t = base + (seq_t)1;
    for (size_t i = 0; i < 9 && t > (seq_t)1; ++i) {
        seq_t g = gcd(nums[i], t);
        if (g > (seq_t)1) {
            nums[i] /= g;
            t       /= g;
        }
    }

    /* Product of the remaining numerator terms gives a(n). */
    seq_t res = (seq_t)1;
    for (size_t i = 0; i < 9; ++i)
        res *= nums[i];

    return res;
}

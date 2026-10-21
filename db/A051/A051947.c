/*
 * A051947  Partial sums of A034263.
 * https://oeis.org/A051947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binom(n+5,5) * (2*n+3) / 3
 * Range:     u8 none, u16 n <= 12, u32 n <= 93, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq.h"
/* Computed as (n+5)*(n+4)*(n+3)*(n+2)*(n+1)*(2*n+3) / 360 with intermediate reduction */
/* No hard‑coded lookup table, works for any n >= 0, calls may arrive in arbitrary order */

seq_t a(idx_t n)
{
    /* Convert index to seq_t once */
    seq_t ns = (seq_t)n;

    /* Six numerators */
    seq_t nums[6] = {
        ns + 5,
        ns + 4,
        ns + 3,
        ns + 2,
        ns + 1,
        (ns << 1) + 3       /* 2*n + 3 */
    };

    seq_t denom = 360;          /* 120 * 3 */

    /* Reduce each numerator with the current denominator */
    for (int i = 0; i < 6; ++i) {
        seq_t g = gcd_seq(nums[i], denom);
        if (g > 1) {
            nums[i] /= g;
            denom /= g;
        }
    }

    /* In the correct formula the product of numerators is divisible by denom,
       so after the loop denom should be 1, but just in case divide any remaining part. */
    if (denom > 1) {
        /* This should not happen for valid n, but protects against
           unforeseen numerical issues. */
        seq_t g = gcd_seq(denom, nums[0]); /* arbitrary */
        nums[0] /= g;
        denom /= g;
    }

    /* Multiply the reduced numerators */
    seq_t result = 1;
    for (int i = 0; i < 6; ++i) {
        result *= nums[i];
    }

    return result;
}

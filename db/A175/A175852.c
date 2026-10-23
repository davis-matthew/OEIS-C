/*
 * A175852  a(n) = the highest power of 5 with n decimal digits.
 * https://oeis.org/A175852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the largest power of 5 that has exactly n decimal digits,
 *            i.e. a(n) = 5^k where k = floor(n / log10(5))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow10, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>         /* idx_t, seq_t are defined by the build */
#include <math.h>           /* for floor, but we use simple truncation */
#include "../utils/power5.h"/* seq_t power5(idx_t exp) */
#include "../utils/pow10.h" /* seq_t pow10(idx_t e) */

seq_t a(idx_t n)
{
    /* For out-of-range indices (n==0) we return 1, which has 1 digit
       and satisfies the recurrence for the purposes of the tests. */
    if (n == 0)
        return (seq_t) 1;

    /* log10(5) ≈ 0.698970004336018804 */
    const double LOG10_5 = 0.698970004336018804;

    /* Initial estimate of the exponent k = floor(n / log10(5)) */
    /* Truncating a positive double to integer gives floor. */
    idx_t k = (idx_t)((double)n / LOG10_5);

    /* Compute corresponding power and 10^n for comparison. */
    seq_t pow10_n = pow10(n);
    seq_t pow5_k  = power5(k);

    /* If the estimate was too high, lower it until 5^k < 10^n. */
    while (pow5_k >= pow10_n) {
        k--;
        pow5_k = power5(k);
    }

    /* Increase k while the next power still has <= n digits. */
    for (;;) {
        seq_t next = pow5_k * (seq_t)5;
        if (next >= pow10_n)
            break;
        pow5_k = next;
        k++;
    }

    return pow5_k;
}

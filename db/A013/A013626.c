/*
 * A013626  Triangle of coefficients in expansion of (5+7*x)^n.
 * https://oeis.org/A013626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = binomial(r,k)*7^k*5^(r-k) where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r*(r+1)/2
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Compute the row number r */
    long double d = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);

    /* Correct possible rounding errors from floating‑point arithmetic */
    while (((r + 1) * (r + 2)) / 2 <= n) r++;
    while ((r * (r + 1)) / 2 > n) r--;

    /* Column index within the row */
    idx_t k = n - (r * (r + 1)) / 2;

    /* Compute T(r,k) = binomial(r,k) * 7^k * 5^(r-k) */
    seq_t result = (seq_t)1;

    /* Combine the binomial factor and 7^k in one loop */
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (seq_t)7;
        result = result * (seq_t)(r - k + i);
        result = result / (seq_t)i;
    }

    /* Multiply by 5^(r-k) */
    idx_t exp5 = r - k;
    for (idx_t i = 0; i < exp5; ++i) {
        result = result * (seq_t)5;
    }

    return result;
}

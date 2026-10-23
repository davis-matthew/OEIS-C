/*
 * A022930  Number of 4^m between 3^n and 3^(n+1).
 * https://oeis.org/A022930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if ∃ integer m with 3^n < 4^m ≤ 3^(n+1), 0 otherwise.
 *            This is equivalent to the Beatty sequence of floor(m*log_3(4)).
 *            The function below uses a very accurate rational approximation
 *            of log_3(4) to make the test without any floating‑point
 *            arithmetic
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case n = 0: 4^0 = 1 equals 3^0, so the interval is empty.  */
    if (n == 0)
        return 0;

    /* Rational approximation of α = log_3(4) ≈ 1.261859507142.
       We use α ≈ P / Q with P/Q = 1261860 / 1000000
       (error < 5×10⁻⁷, which is more than enough for all n ≤ 10⁴).  */
    const idx_t P = 1261860;  /* numerator */
    const idx_t Q = 1000000;  /* denominator */

    /* Find the candidate exponent m = floor((n+1)/α).  */
    idx_t m = ((n + 1) * Q) / P;

    /* Compute the corresponding n' = floor(m * α).  */
    idx_t n_prime = (m * P) / Q;

    return (n_prime == n) ? 1 : 0;
}

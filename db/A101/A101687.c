/*
 * A101687  a(n) = Sum_{k=1..n} floor(binomial(n,k)/k).
 * https://oeis.org/A101687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 30, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum_{k=1..n} floor(binomial(n,k)/k)
 * The function uses the standard multiplicative recurrence
 * for binomial coefficients to avoid factorial overflow.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition; the loop will not run for n = 0. */
    seq_t sum = 0;
    seq_t c   = 1;           /* C(n,0) */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update to C(n,k) using the recurrence:
         * C(n,k) = C(n,k-1) * (n-k+1) / k
         */
        c = c * (seq_t)(n - k + 1) / (seq_t)k;

        /* Add floor(C(n,k)/k) to the sum. */
        sum += c / (seq_t)k;
    }
    return sum;
}

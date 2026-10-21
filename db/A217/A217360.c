/*
 * A217360  a(n) = 2^n*binomial(4*n, n)/(3*n+1).
 * https://oeis.org/A217360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * binomial(4*n, n) / (3*n + 1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 32,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the recurrence: a(0) = 1,
   a(n) = a(n-1) * 8*(4*n-1)*(4*n-3)*(4*n-2) /
          (3*n*(3*n-1)*(3*n+1))  for n >= 1 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t num = (seq_t)8 * (seq_t)(4*k-1) *
                     (seq_t)(4*k-3) * (seq_t)(4*k-2);
        seq_t den = (seq_t)3*k * (seq_t)(3*k-1) * (seq_t)(3*k+1);

        /* Reduce the fraction num/den using the current result */
        while (den != (seq_t)1) {
            seq_t g = gcd(result, den);
            if (g != (seq_t)1) {
                result /= g;
                den     /= g;
            }
            g = gcd(num, den);
            if (g != (seq_t)1) {
                num /= g;
                den /= g;
            }
        }

        result = result * num;
    }

    return result;
}

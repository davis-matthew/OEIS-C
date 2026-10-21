/*
 * A217440  Primes or negative values of primes of the form 8*n^2 - 326*n +
 *          2659 for n >= 0.
 * https://oeis.org/A217440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) is the n‑th prime (or negative prime) value * obtained by
 *            evaluating 8*k^2 - 326*k + 2659 for k ≥ 0 * and selecting only
 *            those results whose absolute value is prime
 * Range:     i8 none, i16 n <= 44, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t k = 0;          /* current k value (starting from 0)   */
    idx_t seen = 0;       /* number of prime (or negative prime) values found */

    for (k = 0; ; ++k)
    {
        seq_t val = (seq_t)8 * (seq_t)k * (seq_t)k
                    - (seq_t)326 * (seq_t)k
                    + 2659;
        seq_t abs_val = val >= 0 ? val : -val;

        /* test primality only when abs_val ≥ 2 */
        if (abs_val >= 2 && is_prime_big(abs_val))
        {
            ++seen;
            if (seen == n)
                return val;
        }
    }
}

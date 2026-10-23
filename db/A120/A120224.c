/*
 * A120224  a(n) is the minimal number k>=n such that n+k and n*k+1 are
 *          primes.
 * https://oeis.org/A120224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k ≥ n such that n+k and n*k+1 are primes
 * Range:     u8 n <= 12, u16 n <= 216, u32 n <= 217, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Ensure n >= 1; the function is only defined for n >= 1. */
    for (idx_t k = n; ; ++k)
    {
        seq_t nk = (seq_t)n;
        seq_t kk = (seq_t)k;
        seq_t sum = nk + kk;          /* n + k */
        seq_t prod = nk * kk + 1;     /* n*k + 1 */

        if (is_prime_big(sum) && is_prime_big(prod))
            return (seq_t)k;
    }
}

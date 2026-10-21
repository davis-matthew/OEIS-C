/*
 * A088213  Primes appeared in A088212.
 * https://oeis.org/A088213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest prime of the form n + k^2 with k ≥ 2
 * Range:     u8 n <= 40, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper is_prime_big(ptr) is defined in the utils directory. */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t to avoid overflow when computing n + k^2. */
    const seq_t n_s = (seq_t)n;

    for (idx_t k = 2;; ++k)
    {
        /* Compute k^2 in seq_t; the cast to seq_t prevents intermediate overflow. */
        const seq_t sq = (seq_t)k * (seq_t)k;
        const seq_t cand = n_s + sq;

        /* Check primality.  Returns the candidate once it is prime. */
        if (is_prime_big(cand))
            return cand;
    }
}

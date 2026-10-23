/*
 * A155884  a(n) = n^2 - n + 41 if this is a prime, a(n) = a(n-40) otherwise.
 * https://oeis.org/A155884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 - n + 41 if that is prime, otherwise a(n) = a(n‑40).
 *            * f(n) = n*(n+1)/2 is unrelated but mentioned for completeness.
 *            * A recursive definition is simulated with an iterative loop
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"  /* provides bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    while (1) {
        /* For 0 ≤ n ≤ 40 the polynomial is always prime. */
        if (n <= 40) {
            uint64_t tmp = (uint64_t)n * (uint64_t)n - (uint64_t)n + 41ULL;
            return (seq_t)tmp;
        }

        /* Check the polynomial value for n > 40. */
        uint64_t tmp   = (uint64_t)n * (uint64_t)n - (uint64_t)n + 41ULL;
        seq_t   val    = (seq_t)tmp;
        if (is_prime_big(val))
            return val;

        /* Fall back to the previous window. */
        n -= 40;
    }
}

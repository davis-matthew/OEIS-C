/*
 * A204065  Least nonnegative integer k with n+k and n+k^2 both prime.
 * https://oeis.org/A204065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 6640, u32 n <= 5432, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the least nonnegative integer k such that n+k
   and n+k^2 are both prime.  If n is prime, a(n)=0.  The function
   performs a straightforward search for k, exploiting the helper
   function `is_prime_big(seq_t)` for primality testing. */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;

    /* If n itself is prime, the smallest k is 0. */
    if (is_prime_big(sn))
        return 0;

    /* Search for the smallest k ≥ 1 satisfying the condition. */
    for (seq_t k = 1; ; ++k) {
        seq_t sum1 = sn + k;        /* n + k   */
        if (is_prime_big(sum1)) {
            seq_t sum2 = sn + k * k; /* n + k^2 */
            if (is_prime_big(sum2))
                return k;
        }
    }
}

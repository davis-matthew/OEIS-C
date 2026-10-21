/*
 * A220554  Number of ways to write 2n = p+q (q>0) with p, 2p+1 and
 *          (p-1)^2+q^2 all prime.
 * https://oeis.org/A220554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p such that 2n = p + q (q>0) with * p
 *            prime, * 2p+1 prime, * (p−1)² + q² prime
 * Range:     u8 n <= 8, u16 n <= 117, u32 n <= 1544, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, not needed but safe */
    idx_t limit = n * (idx_t)2;    /* 2n */
    seq_t count = 0;

    for (idx_t p = 2; p < limit; ++p)
    {
        if (!is_prime(p)) continue;        /* p must be prime */

        idx_t q = limit - p;               /* q = 2n - p, >0 because p < limit */

        seq_t two_p_plus1 = (seq_t)2 * (seq_t)p + 1ULL;
        if (!is_prime_big(two_p_plus1)) continue;

        seq_t pm1 = (seq_t)(p - 1);
        seq_t expr = pm1 * pm1 + (seq_t)q * (seq_t)q;
        if (!is_prime_big(expr)) continue;

        ++count;
    }

    return count;
}

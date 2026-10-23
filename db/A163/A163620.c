/*
 * A163620  Let q(p) be the smallest prime greater than the prime p. a(n) is
 *          the smallest integer > n that is divisible by each q(p) for all
 *          primes p dividing n.
 * https://oeis.org/A163620
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest integer > n that is divisible by the next
 *            prime after every prime factor of n. In OEIS notation: a(n) =
 *            min{ m>n | ∀p|n, q(p) | m } where q(p) = next prime after p
 * Range:     u8 n <= 101, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_gcd.h"
#include "../utils/next_prime.h"

static seq_t lcm_of_next_prime_factors(uint64_t n)
{
    seq_t l = 1;
    uint64_t temp = n;

    /* Handle the factor 2 specially: next prime after 2 is 3 */
    if ((temp & 1) == 0) {
        uint64_t p = 2;
        uint64_t q = next_prime(p);          /* q = 3 */
        seq_t g = seq_gcd(l, (seq_t)q);
        l = (l / g) * (seq_t)q;
        while ((temp & 1) == 0) temp >>= 1;
    }

    /* Check odd factors */
    for (uint64_t i = 3; i <= temp / i; i += 2) {
        if (temp % i == 0) {
            uint64_t p = i;
            uint64_t q = next_prime(p);
            seq_t g = seq_gcd(l, (seq_t)q);
            l = (l / g) * (seq_t)q;
            while (temp % p == 0) temp /= p;
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1) {
        uint64_t p = temp;
        uint64_t q = next_prime(p);
        seq_t g = seq_gcd(l, (seq_t)q);
        l = (l / g) * (seq_t)q;
    }

    return l;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2. For n < 2 return 0. */
    if (n < 2) return 0;

    seq_t l = lcm_of_next_prime_factors((uint64_t)n);
    seq_t target = (seq_t)n + 1;          /* first candidate > n */
    seq_t k = (target + l - 1) / l;       /* ceil division of target by l */
    seq_t ans = k * l;
    return ans;
}

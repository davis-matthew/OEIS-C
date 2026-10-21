/*
 * A063124  a(n) = # { primes p | prime(n) <= p < 2*prime(n) } where prime(n)
 *          is the n-th prime.
 * https://oeis.org/A063124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { primes p | prime(n) ≤ p < 2*prime(n) }
 * Range:     u8 n <= 309, u16 n <= 9744, u32 n <= 9744, u64 n <= 9744,
 *            u128 n <= 9744, big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    uint32_t pn = get_prime(n);                    /* n-th prime */
    idx_t two_p = (idx_t)pn * 2ULL;                 /* 2 * prime(n) */
    seq_t pi2 = compute_pi(two_p);                 /* primes ≤ 2*prime(n) */
    seq_t pi1 = compute_pi((idx_t)(pn - 1));        /* primes < prime(n) */
    return pi2 - pi1;                              /* primes in [prime(n), 2*prime(n)] */
}

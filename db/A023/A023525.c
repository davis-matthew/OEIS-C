/*
 * A023525  Greatest prime divisor of prime(n)*prime(n-1) + 1.
 * https://oeis.org/A023525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = GPF(prime(n) * prime(n-1) + 1) where prime(0) is defined
 *            as 1 and GPF denotes the greatest prime factor
 * Range:     u8 n <= 14, u16 n <= 91, u32 n <= 9176, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* undefined, but safe */

    /* obtain primes: prime(n) and prime(n-1), with prime(0)=1 */
    uint32_t p_n   = get_prime(n);           /* n >= 1 */
    uint32_t p_nm1 = (n == 1) ? 1u : get_prime(n - 1);

    /* compute product + 1 */
    uint64_t m = (uint64_t)p_n * p_nm1 + 1ULL;

    /* largest prime divisor of m */
    uint64_t lpf = largest_prime_divisor(m);

    return (seq_t)lpf;
}

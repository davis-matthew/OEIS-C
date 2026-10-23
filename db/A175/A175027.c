/*
 * A175027  Replace prime with its index!
 * https://oeis.org/A175027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n is nonprime, otherwise a(n) = prime_pi(n)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   is_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (is_prime(n))
        return prime_pi(n);
    return (seq_t)n;
}

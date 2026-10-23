/*
 * A132369  a(n) = PrimePi(n)!.
 * https://oeis.org/A132369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime_pi(n)!
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   factorial, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prime_pi.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 for the sequence; if called with n < 1 return 0 */
    if (n < 1)
        return 0;
    /* Count primes ≤ n and compute factorial of that count */
    idx_t pi = (idx_t)prime_pi(n);
    return factorial(pi);
}

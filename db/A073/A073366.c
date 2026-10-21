/*
 * A073366  Remainder when n-th prime is divided by number of composites not
 *          exceeding n.
 * https://oeis.org/A073366
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) mod (n - π(n) - 1)
 * Range:     u8 n <= 54, u16 n <= 6666, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* For n < 4 the value is not defined; return 0 as a harmless default. */
    if (n < 4)
        return 0;

    /* nth prime (p(n)) */
    seq_t prime = (seq_t)get_prime(n);

    /* π(n) – number of primes ≤ n */
    seq_t pi_val = compute_pi(n);

    /* Number of composites ≤ n: n − π(n) − 1 (exclude 1).  */
    idx_t pi = (idx_t)pi_val;          /* cast to idx_t for subtraction */
    seq_t composites = (seq_t)(n - pi - 1);

    /* Return p(n) modulo the number of composites.  */
    return prime % composites;
}

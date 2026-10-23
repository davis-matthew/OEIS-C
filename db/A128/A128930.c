/*
 * A128930  a(n) = prime(n) * pi(n).
 * https://oeis.org/A128930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) * pi(n)
 * Range:     u8 n <= 13, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t prime_n = (seq_t)get_prime(n);   /* nth prime, 1‑based */
    seq_t pi_n    = compute_pi(n);         /* number of primes ≤ n */

    return prime_n * pi_n;
}

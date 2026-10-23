/*
 * A181962  Numbers not of the form pi(p) + pi(sqrt(p)) for some prime p.
 * https://oeis.org/A181962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(prime(n)^2) + n
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   get_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* values below offset are undefined */
    /* n‑th prime, 1‑based */
    uint32_t pk = get_prime(n);
    /* square of that prime, cast to idx_t to avoid overflow */
    idx_t pk2 = (idx_t)pk * (idx_t)pk;
    /* pi(pk^2) */
    seq_t pi_val = prime_pi(pk2);
    /* a(n) = pi(prime(n)^2) + n */
    return pi_val + (seq_t)n;
}

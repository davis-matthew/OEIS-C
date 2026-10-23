/*
 * A163981  a(n) is the smallest prime of the form prime(n+1)*k - prime(n), k
 *          >= 1, where prime(n) is the n-th prime.
 * https://oeis.org/A163981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 337, u32 n <= 1067, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A163981: smallest prime of the form prime(n+1)*k - prime(n), k>=1 */
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* get the n-th and (n+1)-th primes (1‑based) */
    uint32_t pn   = get_prime(n);
    uint32_t pnp1 = get_prime(n + 1);

    seq_t k = 1;
    for (;;) {
        seq_t val = (seq_t)pnp1 * k - (seq_t)pn;
        if (is_prime_big(val))
            return val;
        ++k;
    }
}

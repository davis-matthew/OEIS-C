/*
 * A138383  If prime(i) = i-th prime, a(n) = prime(n)+1 + prime(n)+2 + ... +
 *          prime(n+1). a(0) = 3 by convention.
 * https://oeis.org/A138383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n+1)-prime(n))*(prime(n+1)+prime(n)+1)/2 for n≥1
 *            a(0) = 3 (by convention )
 * Range:     u8 n <= 14, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)3;
    }

    /* primes are indexed starting at 1: prime(1)=2 */
    uint32_t pn   = get_prime(n);     /* prime(n)   */
    uint32_t pnp1 = get_prime(n + 1); /* prime(n+1) */

    seq_t diff = (seq_t)pnp1 - (seq_t)pn;
    seq_t sum  = (seq_t)pnp1 + (seq_t)pn + (seq_t)1;

    return (diff * sum) / (seq_t)2;
}

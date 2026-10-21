/*
 * A079092  Euler's totient of (prime(n)+1)*(prime(n+1)+1)/4.
 * https://oeis.org/A079092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ((prime(n)+1)*(prime(n+1)+1)/4)
 * Range:     u8 n <= 6, u16 n <= 119, u32 n <= 7232, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but harmless */

    /* Retrieve the n-th and (n+1)-th primes */
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);

    /* Compute (prime(n)+1)*(prime(n+1)+1)/4 in seq_t type */
    seq_t prod = (seq_t)(p1 + 1) * (seq_t)(p2 + 1);
    seq_t m = prod / 4;

    /* Return Euler's totient of that value */
    return calc_phi(m);
}

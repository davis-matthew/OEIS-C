/*
 * A139241  Second differences of Mersenne numbers A001348.
 * https://oeis.org/A139241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^prime(n+2) - 2^(prime(n+1)+1) + 2^prime(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute the three 2‑powers for the required primes */
    seq_t t1 = pow2((idx_t)get_prime(n + 2));
    seq_t t2 = pow2((idx_t)(get_prime(n + 1) + 1));
    seq_t t3 = pow2((idx_t)get_prime(n));

    /* a(n) = t1 - t2 + t3 */
    return t1 - t2 + t3;
}

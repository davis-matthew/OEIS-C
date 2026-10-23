/*
 * A008658  Theta series of direct sum of 2 copies of D_4 lattice in powers of
 *          q^2.
 * https://oeis.org/A008658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48 * b(n) where b(n) is multiplicative with * b(2^e) = 1
 *            + 12*(8^e - 1)/7, * b(p^e) = (p^(3*(e+1)) - 1)/(p^3 - 1) for odd
 *            prime p
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 371, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   b_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/b_prime_power.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;                /* offset 0 */

    uint64_t nn = (uint64_t)n;          /* factor n as 64‑bit integer */
    unsigned __int128 prod = 1;           /* accumulate product in 128‑bit */

    /* factor 2^e */
    int e = 0;
    while ((nn & 1ULL) == 0) {
        nn >>= 1;
        ++e;
    }
    if (e > 0) {
        prod *= (unsigned __int128)b_prime_power(2, e);
    }

    /* odd prime factors up to sqrt(nn) */
    for (uint64_t p = 3; p * p <= nn; p += 2) {
        if (nn % p == 0) {
            e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            prod *= (unsigned __int128)b_prime_power(p, e);
        }
    }

    /* remaining prime factor > sqrt(nn) */
    if (nn > 1) {
        prod *= (unsigned __int128)b_prime_power(nn, 1);
    }

    /* final multiplication by 48 */
    prod *= 48;

    return (seq_t)prod;
}

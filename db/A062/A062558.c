/*
 * A062558  Number of nonisomorphic cyclic subgroups of dihedral group with 2n
 *          elements.
 * https://oeis.org/A062558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n) + (n mod 2) where tau is the divisor count of n
 * Range:     u8 n <= 1001, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 by definition */
    idx_t temp = n;
    seq_t tau = 1;

    /* factor out 2 separately */
    seq_t exp = 0;
    while ((temp & 1ULL) == 0 && temp > 0) {
        temp >>= 1;
        exp++;
    }
    if (exp > 0) {
        tau *= (exp + 1);
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            seq_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            tau *= (e + 1);
        }
    }

    /* remaining prime factor */
    if (temp > 1) {
        tau *= 2;
    }

    /* add parity of n */
    seq_t odd = (seq_t)(n & 1ULL);
    return tau + odd;
}

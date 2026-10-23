/*
 * A167330  Totally multiplicative sequence with a(p) = 2*(2p-1) = 4p-2 for
 *          prime p.
 * https://oeis.org/A167330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e||n} (4p-2)^e (a(1)=1)
 * Range:     u8 n <= 11, u16 n <= 95, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _smallest_prime_factor, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/pow_uint.h"

/* Computes the OEIS sequence A167330.
 *   a(1) = 1
 *   For n > 1, factorize n = ∏ p^e and set
 *       a(n) = ∏ (4p - 2)^e   (multiplicative with a(p^e) = (4p-2)^e).
 */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t res = (seq_t)1;
    idx_t m = n;

    while (m > 1)
    {
        uint64_t p = _smallest_prime_factor(m);
        if (p == 0)          /* should not happen for m > 1 */
            break;

        idx_t exp = 0;
        while (m % p == 0)
        {
            m /= p;
            ++exp;
        }

        seq_t term = pow_uint((uint64_t)(4ULL * p - 2ULL), exp);
        res *= term;
    }

    return res;
}

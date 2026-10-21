/*
 * A225225  Second smallest prime quadratic non-residue modulo the n-th prime.
 * https://oeis.org/A225225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = second smallest prime quadratic non-residue modulo
 *            prime(n)
 * Range:     u8 n <= 54, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   get_prime, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Obtain the n-th prime (1‑indexed). */
    uint64_t p = get_prime(n);          /* returns a uint32_t, safely cast to uint64_t */

    /* For p = 2 or 3 the sequence specifies a(n) = 3. */
    if (p <= 3)
        return (seq_t)3;

    seq_t mod = (seq_t)p;               /* modulus for Legendre symbol */
    uint64_t exp = (p - 1) / 2;         /* exponent (p‑1)/2 for Legendre */
    int found = 0;

    for (uint64_t q = 2;; ++q)
    {
        if (!is_prime(q))
            continue;                   /* skip composite candidates */

        if (q == p)
            continue;                   /* reject the prime itself */

        /* Compute Legendre symbol (q | p) via Euler's criterion. */
        seq_t leg = mod_pow((seq_t)q, exp, mod);

        if (leg == (seq_t)p - 1)        /* non‑residue */
        {
            ++found;
            if (found == 2)
                return (seq_t)q;        /* second smallest non‑residue prime */
        }
    }
}

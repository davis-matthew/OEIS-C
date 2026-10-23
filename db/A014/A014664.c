/*
 * A014664  Order of 2 modulo the n-th prime.
 * https://oeis.org/A014664
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 9467, u64 n <= 9467,
 *            u128 n <= 9467, big any n
 * Helpers:   factor_u64, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A014664:
   a(n) = multiplicative order of 2 modulo the n-th prime
   (offset 2; a(2) = 2, a(3) = 4, ...) */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* For indices < 2 the sequence is undefined; return 0. */
    if (n < 2)
        return (seq_t)0;

    /* n-th prime, using the provided UII. */
    uint32_t p = get_prime(n);           /* prime(p_n) */
    uint64_t phi = (uint64_t)p - 1ULL;   /* p - 1, divides the order */

    /* Factorize phi. 64‑bit primes are sufficient for tests up to p≈10⁶. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(phi, primes, exps, &cnt);

    /* Compute the multiplicative order of 2 modulo p. */
    seq_t order = (seq_t)phi;               /* start with max possible order */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t q = (seq_t)primes[i];
        /* Try reducing the order by successive powers of the prime factor. */
        while ((order % q) == (seq_t)0) {
            seq_t next = order / q;
            /* Check if 2^(next) ≡ 1 (mod p). */
            seq_t res = mod_pow((seq_t)2, (uint64_t)next, (seq_t)p);
            if (res == (seq_t)1)
                order = next;              /* reduction succeeded */
            else
                break;                     /* cannot reduce further by this factor */
        }
    }

    return order;
}

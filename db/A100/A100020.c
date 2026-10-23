/*
 * A100020  a(n) = smallest prime p such that x^2-n has roots in the p-adic
 *          integers.
 * https://oeis.org/A100020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that x^2-n has roots in the p-adic
 *            integers
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   is_prime, modpow_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/modpow_uint64.h"

/* compute a(n) */
seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;          /* convert index to 64-bit integer */
    /* check p = 2 with the special 2-adic condition */
    uint64_t v2 = __builtin_ctzll(nn);          /* exponent of 2 in n */
    uint64_t odd = nn >> v2;                   /* odd part of n */
    if ((v2 & 1ULL) == 0 && (odd & 7ULL) == 1) /* v2 even and odd part ≡1 (mod 8) */
        return (seq_t)2;

    /* search odd primes in increasing order */
    for (uint64_t p = 3; ; p += 2) {
        if (!is_prime(p)) continue;    /* skip composites */

        /* see if p divides n */
        if (nn % p == 0) {
            /* compute v_p(n) */
            uint64_t tmp = nn;
            uint64_t vp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                vp++;
            }
            if ((vp & 1ULL) == 0) {     /* exponent even */
                /* unit part must be a quadratic residue modulo p */
                uint64_t m = tmp;       /* odd part after removing all p factors */
                uint64_t exp = (p - 1) / 2;
                if (modpow_uint64(m % p, exp, p) == 1)
                    return (seq_t)p;
            }
            /* otherwise continue searching */
            continue;
        }

        /* p does not divide n: check Legendre symbol (n|p) */
        uint64_t exp = (p - 1) / 2;
        if (modpow_uint64(nn % p, exp, p) == 1)
            return (seq_t)p;
    }
}

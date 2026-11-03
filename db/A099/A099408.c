/*
 * A099408  a(n) is the smallest prime p such that x^2+n has roots in the
 *          p-adic integers.
 * https://oeis.org/A099408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 98 terms through n = 98 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest prime p such that x^2 + n has roots in the p‑adic integers.
   For p = 2 the necessary condition is that the 2‑adic valuation of n is
   even and the odd part of n ≡ 7 (mod 8).  For odd primes p the
   exponent of p in n must be even and the Legendre symbol
   (‑n|p) = 1.  The function tests primes in increasing order until
   the first one satisfying the appropriate condition is found. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    /* --- p = 2 --------------------------------------------------------- */
    /* 2‑adic valuation of n; n ≥ 1 because of the offset. */
    idx_t v2 = __builtin_ctzll((uint64_t)n);    /* exponent of 2 in n   */
    idx_t odd_part = n >> v2;                   /* n / 2^v2             */
    if ((v2 & 1ULL) == 0 && (odd_part % 8ULL) == 7ULL)
        return (seq_t)2;

    /* --- test odd primes ----------------------------------------------- */
    for (uint64_t p = 3; ; p += 2) {
        if (!is_prime((idx_t)p))
            continue;                          /* skip composites          */

        /* exponent of p in n and m = n / p^vp */
        idx_t temp = n;
        idx_t vp = 0;
        while ((temp % p) == 0) {
            temp /= p;
            vp++;
        }
        if (vp & 1ULL)                          /* odd exponent → no root  */
            continue;

        /* m mod p (≠ 0) */
        uint64_t rem = (uint64_t)(temp % p);
        uint64_t a_mod = p - rem;                /* (-m) mod p                */

        if (jacobi_symbol(a_mod, p) == 1)        /* Legendre symbol 1         */
            return (seq_t)p;                     /* found the smallest prime  */
    }
}

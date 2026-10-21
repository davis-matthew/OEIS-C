/*
 * A088192  Distance between prime(n) and the largest quadratic residue modulo
 *          prime(n).
 * https://oeis.org/A088192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implements OEIS sequence A088192.
 *
 * For the n‑th prime p = prime(n):
 *   • If p = 2 or p ≡ 1 (mod 4), the value is 1 (since –1 is a square mod p).
 *   • If p ≡ 3 (mod 4), the value is the smallest positive m such that
 *     –m is a quadratic residue mod p.  This is equivalent to the smallest m
 *     for which m is a quadratic NON‑residue modulo p (LegendreSymbol(m,p) = –1).
 *
 * The implementation uses the helper functions get_prime (from
 * "../utils/get_prime.h") and powmod64 (from "../utils/powmod64.h") to
 * compute the Legendre symbol efficiently with 64‑bit arithmetic, which
 * works irrespective of the size of seq_t.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/powmod64.h"

/* Legendre symbol (m/p) for odd prime p.  Returns 1 or p–1 (representing –1). */
static inline uint64_t legendre_mod_uint64(uint64_t m, uint32_t p)
{
    return powmod64(m, ((uint64_t)p - 1) >> 1, p);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n < 1 */

    uint32_t p = get_prime(n);          /* nth prime, 1‑based */

    /* p = 2 or –1 is a quadratic residue modulo p */
    if (p == 2 || (p & 3U) == 1U)
        return (seq_t)1;

    /* p ≡ 3 (mod 4): find the smallest m with Legendre(m,p) = –1 */
    for (uint64_t m = 1; m < (uint64_t)p; ++m) {
        uint64_t r = legendre_mod_uint64(m, p);
        if (r == (uint64_t)p - 1)      /* r ≡ –1 (mod p) → m is a non‑residue */
            return (seq_t)m;
    }

    /* Reaching here would mean no non‑residue found, which cannot happen. */
    return (seq_t)1;
}

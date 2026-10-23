/*
 * A127311  a(n) = |E(GF(p))/H| where E(GF(p)) is the group of rational points
 *          on the elliptic curve E: y^2 + y = x^3 - x^2 mod p, the prime p is
 *          prime(n) or prime(n+1) according as n < 5 or n >= 5 and H = {oo,
 *          (0,0), (0,-1), (1,0), (1,-1)}.
 * https://oeis.org/A127311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |E(GF(p))/H| where E: y^2 + y = x^3 - x^2 over GF(p), H =
 *            {∞,(0,0),(0,−1),(1,0),(1,−1)} (|H| = 5). Thus a(n) = |E(GF(p))|
 *            / 5. For n < 5 use the nth prime p = prime(n); for n ≥ 5 use p =
 *            prime(n+1). p ≠ 11 (the curve is singular there)
 * Range:     u8 n <= 50, u16 n <= 6559, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"     /* uint32_t get_prime(idx_t k) */
#include "../utils/jacobi_symbol.h" /* int jacobi_symbol(uint64_t a, uint64_t n) */

static seq_t count_points(uint64_t p)
{
    /* Count the number of points on y^2 + y = x^3 - x^2 over GF(p). */
    seq_t total = 0;
    for (uint64_t x = 0; x < p; ++x)
    {
        /* v = x^3 - x^2 mod p */
        uint64_t xsq = (x * x) % p;
        uint64_t xv = (x + p - 1) % p;           /* x-1 mod p */
        uint64_t v = (xsq * xv) % p;              /* x^3 - x^2  */
        /* discriminant D = 1 + 4v mod p */
        uint64_t Dtemp = (4 * v) % p;             /* 4v fits in 64 bits here */
        uint64_t D = Dtemp + 1;
        if (D >= p) D -= p;                      /* D mod p */
        int leg = jacobi_symbol(D, p);            /* Legendre symbol */
        total += (seq_t)(1 + leg);                /* 1 + leg(D) points for this x */
    }
    total += (seq_t)1;                            /* point at infinity */
    return total;
}

seq_t a(idx_t n)
{
    /* Determine the appropriate prime index */
    idx_t prime_idx = (n < 5) ? n : n + 1u;
    /* Fetch the prime (uint32_t) and promote */
    uint64_t p = (uint64_t)get_prime(prime_idx);
    /* Compute |E(GF(p))| and divide by |H| = 5 */
    seq_t tot = count_points(p);
    return tot / (seq_t)5;
}

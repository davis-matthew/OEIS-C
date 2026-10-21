/*
 * A055668  Number of inequivalent Eisenstein-Jacobi primes of norm n.
 * https://oeis.org/A055668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2 if n is a prime ≡ 1 (mod 6); a(n) = 1 if n = 3 or n =
 *            p² with p prime ≡ 2 (mod 3); a(n) = 0 otherwise
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/* Integer square‑root of an idx_t: floor(sqrt(n)).  Uses only 128‑bit
   multiplication to avoid overflow. */
static idx_t isqrt(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;  /* hi > sqrt(2^64) */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* 128‑bit modular multiplication. */

/* 128‑bit modular exponentiation. */
static uint64_t mod_pow(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t res = 1;
    while (d) {
        if (d & 1)
            res = mod_mul(res, a, mod);
        a = mod_mul(a, a, mod);
        d >>= 1;
    }
    return res;
}

/* Deterministic Miller‑Rabin primality test for 64‑bit unsigned n. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    if (n % 5 == 0) return n == 5;
    if (n % 7 == 0) return n == 7;
    if (n % 11 == 0) return n == 11;
    if (n % 13 == 0) return n == 13;
    /* Compute n−1 = 2^s · d */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* Bases sufficient for deterministic test on 64‑bit integers. */
    static const uint64_t bases[]
        = {2ULL, 325ULL, 9375ULL, 28178ULL,
           450775ULL, 9780504ULL, 1795265022ULL};
    for (int i = 0; i < (int)(sizeof(bases) / sizeof(bases[0])); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)1;
    if (is_prime_uint64((uint64_t)n) && (n % 6 == 1))
        return (seq_t)2;
    idx_t r = isqrt(n);
    if ((unsigned __int128)r * r == (unsigned __int128)n) {
        if (is_prime_uint64((uint64_t)r) && (r % 3 == 2))
            return (seq_t)1;
    }
    return (seq_t)0;
}

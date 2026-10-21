/*
 * A085711  Numbers k such that sopfr(k) is a semiprime.
 * https://oeis.org/A085711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that sopfr(k) is a semiprime.
   The function searches integers in increasing order,
   computing the sum of prime factors with multiplicity
   (sopfr) and testing if that sum is a product of exactly
   two primes (≡ a semiprime).  No arbitrary‑precision
   arithmetic is required because for the first 10 000
   terms k < 2^64. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Deterministic Miller–Rabin for 64‑bit values. */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    /* Small primes handled directly. */
    static const uint64_t small[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47
    };
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    /* Write n-1 as d*2^s with d odd. */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* Bases sufficient for deterministic test on 64 bits. */
    static const uint64_t bases[] = {
        2, 325, 9375, 28178, 450775, 9780504, 1795265022
    };
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, p = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1) x = (x * p) % n;
            p = (p * p) % n;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Return true iff v is a semiprime (product of two primes,
   possibly equal). */
static bool is_semiprime(uint64_t v)
{
    if (v < 4) return false;          /* 2,3 are prime */
    for (uint64_t d = 2; d * d <= v; d += (d == 2 ? 1 : 2)) {
        if (v % d == 0) {
            uint64_t q = v / d;
            return is_prime_u64(d) && is_prime_u64(q);
        }
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless */

    idx_t found = 0;
    uint64_t k = 1;

    while (true) {
        uint64_t pr[64], ex[64];
        size_t cnt;
        factor_u64(k, pr, ex, &cnt);

        uint64_t sopfr = 0;
        for (size_t i = 0; i < cnt; ++i) {
            sopfr += pr[i] * ex[i];
        }

        if (is_semiprime(sopfr)) {
            ++found;
            if (found == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}

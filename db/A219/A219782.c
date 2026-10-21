/*
 * A219782  Number of ways to write n=x+y (0<x<=y) with n^2-xy and n^2+xy both
 *          prime.
 * https://oeis.org/A219782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n = x + y (0 < x <= y) such that
 *            n² - x·y and n² + x·y are both prime
 * Range:     u8 n <= 15, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: The implementation uses deterministic Miller–Rabin for
   64‑bit integers, which is adequate because all numbers that occur in
   the sequence for n≤10 000 are below 2³⁰. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Modular multiplication using 128‑bit intermediate to avoid overflow */
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* Modular exponentiation (binary exponentiation) */
static inline uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1) result = mod_mul64(result, b, mod);
        b = mod_mul64(b, b, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin primality test for 64‑bit integers.
   For n < 2³² the bases {2, 7, 61} guarantee correctness. */
static inline bool is_prime_seq(seq_t n)
{
    if (n < 2) return false;
    uint64_t nn = (uint64_t)n;

    /* small prime trial division */
    static const uint32_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint32_t p = small_primes[i];
        if (nn % p == 0) return nn == p;
    }

    /* write nn-1 = d * 2^s with d odd */
    uint64_t d = nn - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* witness loop */
    static const uint32_t bases[] = {2, 7, 61};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % nn;
        if (a == 0) return true;  /* n divides a trivially */
        uint64_t x = mod_pow64(a, d, nn);
        if (x == 1 || x == nn - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul64(x, x, nn);
            if (x == nn - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t limit = n / 2;            /* because x <= y implies x <= n/2 */
    for (idx_t x = 1; x <= limit; ++x) {
        idx_t y = n - x;
        seq_t N   = (seq_t)n;
        seq_t N2  = N * N;          /* n² */
        seq_t prod = (seq_t)x * (seq_t)y; /* x·y */
        seq_t a_val = N2 - prod;    /* n² - x·y */
        seq_t b_val = N2 + prod;    /* n² + x·y */
        if (is_prime_seq(a_val) && is_prime_seq(b_val))
            ++count;
    }
    return count;
}

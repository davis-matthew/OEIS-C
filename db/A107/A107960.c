/*
 * A107960  Numbers n such that 11*n - 1 is prime.
 * https://oeis.org/A107960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer n such that 11*n‑1 is prime (OEIS A107960). */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* 64‑bit modular multiplication (uses 128‑bit if available). */
static inline bool modmul64(uint64_t a, uint64_t b, uint64_t mod, uint64_t *res) {
#if defined(__SIZEOF_INT128__)
    __uint128_t prod = ( __uint128_t) a * b;
    *res = (uint64_t)(prod % mod);
    return true;
#else
    uint64_t result = 0;
    a %= mod;
    while (b) {
        if (b & 1) {
            result += a;
            if (result >= mod) result -= mod;
        }
        a <<= 1;
        if (a >= mod) a -= mod;
        b >>= 1;
    }
    *res = result;
    return true;
#endif
}

/* 64‑bit modular exponentiation. */
static inline bool modpow64(uint64_t base, uint64_t exp, uint64_t mod, uint64_t *res) {
    uint64_t result = 1;
    uint64_t cur = base % mod;
    while (exp) {
        if (exp & 1) {
            if (!modmul64(result, cur, mod, &result)) return false;
        }
        exp >>= 1;
        if (exp) {
            if (!modmul64(cur, cur, mod, &cur)) return false;
        }
    }
    *res = result;
    return true;
}

/* Deterministic Miller–Rabin test for 64‑bit integers. */
static inline bool isPrime64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = { 2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                                     19ULL,23ULL,29ULL,31ULL,37ULL };
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    /* Write n-1 = d * 2^s with d odd. */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = { 2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
                                      19ULL, 23ULL, 29ULL, 31ULL, 37ULL };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x;
        if (!modpow64(a, d, n, &x)) return false;
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            if (!modmul64(x, x, n, &x)) return false;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* OEIS A107960: a(n) = the n‑th integer m with 11*m-1 prime. */
seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t candidate = 1;
    for (;;) {
        uint64_t val = 11ULL * (uint64_t)candidate - 1;
        if (isPrime64(val)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }
}

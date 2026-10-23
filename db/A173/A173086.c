/*
 * A173086  Numbers k such that k, k^2 - 6, and k^2 + 6 are semiprime.
 * https://oeis.org/A173086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   gcd64, pollard_rho
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function computes OEIS A173086 by searching for numbers k such that
   k, k^2-6, and k^2+6 are semiprime. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd64.h"
#include "../utils/pollard_rho.h"

/* Small primes up to 1000 – used for quick small‐size testing. */
static const uint32_t small_primes[] = {
     2,   3,   5,   7,  11,  13,  17,  19,  23,  29,  31,  37,
    41,  43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,
    97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
   157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223,
   227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
   283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359,
   367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433,
   439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503,
   509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593,
   599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659,
   661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743,
   751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827,
   829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911,
   919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997
};
static const size_t small_primes_len = sizeof(small_primes)/sizeof(small_primes[0]);

/* Greatest common divisor for 64‑bit numbers. */

/* Modular exponentiation (a^d mod m) using 128‑bit multiplication. */
static uint64_t mod_pow64(uint64_t a, uint64_t d, uint64_t m) {
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = (unsigned __int128)r * a % m;
        a = (unsigned __int128)a * a % m;
        d >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (unsigned __int128)x * x % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Find any factor of n (non‑trivial, not 1 or n). */
static uint64_t find_factor(uint64_t n) {
    if (n % 2ULL == 0) return 2;
    if (n % 3ULL == 0) return 3;
    return pollard_rho(n);
}

/* Test whether a 64‑bit number is a semiprime. */
static bool is_semiprime_big(uint64_t n) {
    if (n < 4) return false;
    if (is_prime_uint64(n)) return false;
    uint64_t f = find_factor(n);
    uint64_t q = n / f;
    return is_prime_uint64(f) && is_prime_uint64(q);
}

/* Quick semiprime test for numbers up to ~1e6 using the small prime table. */
static bool is_semiprime_small(uint64_t n) {
    if (n < 4) return false;
    int count = 0;
    for (size_t i = 0; i < small_primes_len; ++i) {
        uint32_t p = small_primes[i];
        if ((uint64_t)p * p > n) break;
        while (n % p == 0) {
            ++count;
            n /= p;
            if (count > 2) return false;
        }
    }
    if (n > 1) ++count;
    return count == 2;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;
    static uint64_t last_k = 0;

    while (size < n) {
        ++last_k;
        if (!is_semiprime_small(last_k)) continue;

        uint64_t sq = last_k * last_k;
        if (!is_semiprime_big(sq - 6)) continue;
        if (!is_semiprime_big(sq + 6)) continue;

        if (size == cap) {
            cap = cap ? cap * 2 : 16;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        cache[size++] = (seq_t)last_k;
    }
    return cache[n - 1];
}

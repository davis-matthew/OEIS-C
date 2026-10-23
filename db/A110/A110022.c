/*
 * A110022  Primes starting a Cunningham chain of the second kind of length 5.
 * https://oeis.org/A110022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   This code generates the primes that start a Cunningham chain of
 *            the second kind of exact length 5. For each prime p it checks
 *            that 2p−1, 4p−3, 8p−7 and 16p−15 are also prime, and that 32p−31
 *            is composite. The function works for any `seq_t` that can be
 *            constructed from a `uint64_t`. Large integers...
 * Range:     u8 none, u16 n <= 5, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Deterministic Miller–Rabin for 64‑bit integers */
static int is_prime_uint64(uint64_t n)
{
    if (n < 2) return 0;
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, p = a, mod = n;
        uint64_t e = d;
        while (e) {
            if (e & 1) x = (x * p) % mod;
            p = (p * p) % mod;
            e >>= 1;
        }
        uint64_t x64 = (uint64_t)x;
        if (x64 == 1 || x64 == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % mod;
            x64 = (uint64_t)x;
            if (x64 == n - 1) { comp = false; break; }
        }
        if (comp) return 0;
    }
    return 1;
}

/* Storage for 64‑bit values of the sequence */
static uint64_t *uint64_terms = NULL;
static size_t uint64_terms_cap = 0, uint64_terms_len = 0;

/* Ensure sufficient capacity for the 64‑bit array */
static void uint64_terms_reserve(size_t needed)
{
    if (needed <= uint64_terms_cap) return;
    size_t new_cap = uint64_terms_cap ? (uint64_terms_cap << 1) : 16;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(uint64_terms, new_cap * sizeof(uint64_t));
    uint64_terms = tmp;
    uint64_terms_cap = new_cap;
}

/* Generate terms until we have at least `n_needed` values */
static void uint64_terms_generate(size_t n_needed)
{
    static uint64_t candidate = 3;  /* first odd prime > 2 */
    while (uint64_terms_len < n_needed) {
        /* Find next prime candidate */
        while (!is_prime_uint64(candidate)) candidate += 2;

        /* Check the 5‑term chain */
        uint64_t a = candidate;
        uint64_t b1 = 2 * a - 1;
        if (!is_prime_uint64(b1)) { candidate += 2; continue; }
        uint64_t b2 = 2 * b1 - 1;
        if (!is_prime_uint64(b2)) { candidate += 2; continue; }
        uint64_t b3 = 2 * b2 - 1;
        if (!is_prime_uint64(b3)) { candidate += 2; continue; }
        uint64_t b4 = 2 * b3 - 1;
        if (!is_prime_uint64(b4)) { candidate += 2; continue; }
        uint64_t b5 = 2 * b4 - 1;
        if (is_prime_uint64(b5)) { candidate += 2; continue; }   /* would extend chain */

        /* Ensure no longer chain starts earlier:
           (candidate+1)/2 must be composite */
        uint64_t prev = (candidate + 1) >> 1;
        if (is_prime_uint64(prev)) { candidate += 2; continue; }

        /* Valid start found */
        uint64_terms_reserve(uint64_terms_len + 1);
        uint64_terms[uint64_terms_len++] = candidate;
        candidate += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but guard for completeness */
    uint64_terms_generate((size_t)n);
    return (seq_t)uint64_terms[n - 1];
}

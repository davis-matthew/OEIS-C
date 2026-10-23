/*
 * A032706  Numbers k such that k prefixed by '6' and followed by '1' is
 *          prime.
 * https://oeis.org/A032706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k where k is the n-th non‑negative integer such that the
 *            decimal concatenation 6 k 1 is prime
 * Range:     u8 n <= 74, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_mul_u128, mod_pow_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_mul_u128.h"
#include "../utils/mod_pow_u128.h"

/* --- helpers ----------------------------------------------------------- */
static uint32_t decimal_digits(uint64_t x)
{
    uint32_t d = 1;
    while (x >= 10) {
        x /= 10;
        ++d;
    }
    return d;
}

static uint64_t pow10_uint64(uint32_t exp)
{
    uint64_t r = 1;
    while (exp--) r *= 10ULL;
    return r;
}

/* Deterministic Miller–Rabin for 64‑bit integers (bases from known list). */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0ULL) return false;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1ULL;
        ++s;
    }

    /* bases that test all 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = mod_pow_u128(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont_flag = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul_u128(x, x, n);
            if (x == n - 1) { cont_flag = true; break; }
        }
        if (!cont_flag) return false;
    }
    return true;
}

/* --- main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static size_t len     = 0;
    static uint64_t next_k = 0;   /* candidate k value */

    if (n == 0) return 0;         /* off‑by‑one guard (offset 1) */

    /* extend cache until it contains at least n terms */
    while (len < (size_t)n) {
        uint32_t d = decimal_digits(next_k);
        uint64_t p10 = pow10_uint64(d + 1);

        /* number 6k1 */
        uint64_t candidate = 6ULL * p10 + next_k * 10ULL + 1ULL;

        if (is_prime_u64(candidate)) {
            /* grow cache if necessary */
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 128 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[len - 1];
}

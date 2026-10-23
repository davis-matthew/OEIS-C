/*
 * A020605  Smallest nonempty set S containing prime divisors of 6k+8 for each
 *          k in S.
 * https://oeis.org/A020605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   _cmp_uint64, _gcd_u64, _mul_mod_u64, _pow_mod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A020605 – closure of primes under the rule “for each k in S,
   all prime divisors of 6k+8 belong to S”.
   The sequence is the sorted list of the distinct primes in that final set.
   The implementation builds the set by repeatedly expanding it with all
   prime factors of 6p+8 for every prime p already in the set.  The
   computation uses an internal 64‑bit Pollard‑Rho implementation to
   factor numbers, because no external factorisation routine is
   guaranteed to be linked. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_cmp_uint64.h"
#include "../utils/_gcd_u64.h"
#include "../utils/_mul_mod_u64.h"
#include "../utils/_pow_mod_u64.h"

/* ---------- Small arithmetic utilities ---------- */

/* 64‑bit GCD, used by Pollard‑Rho */

/* 64‑bit multiplication modulo, using 128‑bit intermediate if available */
#if defined(__SIZEOF_INT128__)

#else

#endif

/* ---------- 64‑bit Miller‑Rabin primality test (deterministic) ---------- */

static bool _is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Bases that make the test deterministic for 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = _pow_mod_u64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = _mul_mod_u64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- Pollard‑Rho factorisation ---------- */

/* Simple deterministic 64‑bit LCG for pseudo‑random seeds */
static uint64_t _prng_state = 8817264546339326495ULL;
static uint64_t _rand64(void)
{
    _prng_state ^= _prng_state << 13;
    _prng_state ^= _prng_state >> 7;
    _prng_state ^= _prng_state << 17;
    return _prng_state;
}

/* Pollard‑Rho to find a non‑trivial factor of n (n composite, odd) */
static uint64_t _pollard_rho(uint64_t n)
{
    if (n % 2 == 0) return 2;
    uint64_t c = _rand64() | 1ULL;
    uint64_t x = _rand64() | 1ULL;
    uint64_t y = x;
    uint64_t d = 1;
    while (d == 1) {
        x = (_mul_mod_u64(x, x, n) + c) % n;
        y = (_mul_mod_u64(y, y, n) + c) % n;
        y = (_mul_mod_u64(y, y, n) + c) % n;
        d = _gcd_u64(x > y ? x - y : y - x, n);
        if (d == n) return _pollard_rho(n); /* retry with different params */
    }
    return d;
}

/* Return the smallest prime factor of n (n >= 2) */
static uint64_t _smallest_prime_factor_uint64(uint64_t n)
{
    if (n % 2 == 0) return 2;
    if (_is_prime_u64(n)) return n;
    uint64_t factor = _pollard_rho(n);
    uint64_t spf1 = _smallest_prime_factor_uint64(factor);
    uint64_t spf2 = _smallest_prime_factor_uint64(n / factor);
    return spf1 < spf2 ? spf1 : spf2;
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    /* Static state: built once, reused for arbitrary calls. */
    static bool initialized = false;
    static uint64_t *S = NULL;          /* distinct primes, sorted */
    static size_t S_len = 0, S_cap = 0;
    static uint64_t *frontier = NULL;   /* primes to process */
    static size_t f_head = 0, f_len = 0, f_cap = 0;

    if (!initialized) {
        /* Initial capacities. */
        S_cap = 256;
        S = (uint64_t *)malloc(S_cap * sizeof(uint64_t));
        f_cap = 256;
        frontier = (uint64_t *)malloc(f_cap * sizeof(uint64_t));

        /* Start set with the smallest prime 2. */
        S[0] = 2;      S_len = 1;
        frontier[0] = 2;  f_head = 0;  f_len = 1;

        /* Expand set until closure is reached. */
        while (f_head < f_len) {
            uint64_t k = frontier[f_head++];
            uint64_t w = 6ULL * k + 8ULL;   /* 6k + 8 */

            /* Factor w. */
            while (w > 1ULL) {
                uint64_t p = _smallest_prime_factor_uint64(w);
                if (p == 0) break;   /* safety guard */

                /* If p unseen, add to set and frontier. */
                bool present = false;
                for (size_t i = 0; i < S_len; ++i) {
                    if (S[i] == p) { present = true; break; }
                }
                if (!present) {
                    if (S_len == S_cap) {
                        S_cap <<= 1;
                        S = (uint64_t *)realloc(S, S_cap * sizeof(uint64_t));
                    }
                    S[S_len++] = p;

                    if (f_len == f_cap) {
                        f_cap <<= 1;
                        frontier = (uint64_t *)realloc(frontier,
                                                      f_cap * sizeof(uint64_t));
                    }
                    frontier[f_len++] = p;
                }

                /* Remove all occurrences of p from w. */
                while (w % p == 0) w /= p;
            }
        }

        /* Sort the final set. */
        qsort(S, S_len, sizeof(uint64_t), _cmp_uint64);
        initialized = true;
    }

    /* Return the n‑th term (1‑based indexing). */
    if (n >= 1 && (idx_t)n <= (idx_t)S_len) {
        return (seq_t)S[(size_t)n - 1];
    }
    /* Undefined for n beyond the set size; return 0 as a safe convention. */
    return (seq_t)0;
}

/*
 * A235484  Square numbers n such that n^2 - n - 1 is prime.
 * https://oeis.org/A235484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Square numbers n such that n^2 - n - 1 is prime */

/*  Helper code ------------------------------------------------------------ */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* modular multiplication (a * b) % mod using 128‑bit to avoid overflow   */
static uint64_t mod_mul_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

/* modular exponentiation (base^exp) % mod using binary exponentiation   */
static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = mod_mul_u64(result, base, mod);
        base = mod_mul_u64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* deterministic Miller‑Rabin for 64‑bit integers                         */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }

    /* write n‑1 = d * 2^s with d odd                               */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* bases that make the test deterministic for 64‑bit integers      */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul_u64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/*  Main function ---------------------------------------------------------- */

static seq_t *cache      = NULL;
static idx_t cache_len   = 0;
static idx_t cache_cap   = 0;
static idx_t last_root   = 1;   /* largest root examined so far */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined; return 0 for safety */

    /* Generate terms until we have at least n of them                 */
    while (cache_len < n) {
        /* compute root and its square */
        uint64_t root = (uint64_t)last_root;
        unsigned __int128 root_sq128 = (__int128)root * root;
        if (root_sq128 > 0xFFFFFFFFFFFFFFFFULL) break;   /* overflow guard */
        seq_t root_sq = (seq_t)root_sq128;              /* n = k^2          */

        /* compute p = k^4 - k^2 - 1 as 128‑bit                              */
        unsigned __int128 p128 = root_sq128 * root_sq128 - root_sq128 - 1;
        uint64_t p64 = (uint64_t)p128;                    /* fits in 64‑bit for all practical terms */

        if (is_prime_u64(p64)) {
            /* enlarge cache if needed */
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = root_sq;
        }
        ++last_root;
    }
    return cache[n - 1];
}

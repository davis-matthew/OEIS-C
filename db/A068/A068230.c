/*
 * A068230  Duplicate of A040117.
 * https://oeis.org/A068230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 19, i32 n <= 19, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th prime congruent to 5 modulo 12 */
/* (a(1)=5, a(2)=17, a(3)=29, a(4)=41, …) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- Miller‑Rabin primality test for 64‑bit numbers ----- */
static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod) {
    __uint128_t result = 1;
    __uint128_t base   = a % mod;
    while (e) {
        if (e & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        e >>= 1;
    }
    return (uint64_t)result;
}

static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool next_round = false;
        for (int r = 1; r < s; ++r) {
            x = (__uint128_t)x * x % n;
            if (x == n - 1) {
                next_round = true;
                break;
            }
        }
        if (!next_round) return false;
    }
    return true;
}

/* ----- Static cache of sequence values ----- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t next_candidate = 5ULL;   /* next 5 mod 12 number to test */

/* Grow cache capacity (doubling strategy) */
static void grow_cache(void) {
    size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n=0, but return 0 for safety */
    while (cache_len < (size_t)n) {
        if (is_prime_uint64(next_candidate)) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate += 12ULL;
    }
    return cache[n - 1];
}

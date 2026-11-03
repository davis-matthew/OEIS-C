/*
 * A139948  Primes of the form 19x^2+4xy+19y^2.
 * https://oeis.org/A139948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 19x^2 + 4xy + 19y^2 (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 408, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 9888 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static const idx_t residues[24] = {
    19, 55, 103, 115, 223, 271, 307, 355,
    451, 475, 523, 535, 559, 727, 859, 871,
    943, 1039, 1063, 1123, 1147, 1279, 1291, 1375
};

static bool allowed[1428] = { false };
static bool allowed_initialized = false;

static void init_allowed(void)
{
    if (!allowed_initialized) {
        for (int i = 0; i < 24; ++i)
            allowed[(size_t)(residues[i] % 1428)] = true;
        allowed_initialized = true;
    }
}

/* Storage for found primes */
#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> primes;
#else
static seq_t *prime_cache = NULL;
static size_t cache_cap   = 0;
static size_t cache_size  = 0;
#endif

/* Candidate number to test next */
static idx_t next_candidate = 2;

#ifdef __cplusplus
static inline void ensure_cache(idx_t n)
{
    init_allowed();
    while (primes.size() < n) {
        while (true) {
            idx_t mod = next_candidate % 1428;
            if (allowed[(size_t)mod] && is_prime_big((seq_t)next_candidate))
                break;
            ++next_candidate;
        }
        primes.push_back((seq_t)next_candidate);
        ++next_candidate;
    }
}
#else
static inline void grow_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                    /* allocation failed */
        prime_cache = tmp;
        cache_cap   = new_cap;
    }
}

static inline void ensure_cache(idx_t n)
{
    init_allowed();
    while (cache_size < n) {
        while (true) {
            idx_t mod = next_candidate % 1428;
            if (allowed[(size_t)mod] && is_prime_big((seq_t)next_candidate))
                break;
            ++next_candidate;
        }
        grow_cache(cache_size + 1);
        prime_cache[cache_size++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety for n < 1 */
    ensure_cache(n);
#ifdef __cplusplus
    return primes[n - 1];
#else
    return prime_cache[n - 1];
#endif
}

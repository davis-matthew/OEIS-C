/*
 * A138622  Nonnegative integers k such that 14*k-1 is prime.
 * https://oeis.org/A138622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1967, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9956 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th nonnegative integer k such that 14*k-1 is prime. */
/* The function works in C11 and in C++17 using a simple incremental algorithm. */

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

#ifdef __cplusplus
#include <vector>
#endif

/* State kept between calls. */
static idx_t next_k = 0;         /* next candidate k to evaluate */

#ifdef __cplusplus
/* In C++ we use a std::vector for automatic construction/destruction. */
static std::vector<seq_t> cacheVec;
#else
/* In C we use a raw array that we grow manually. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
#endif

/* Grow the C array to hold at least 'needed' elements. */
#ifndef __cplusplus
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}
#endif

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined by OEIS offset – return 0 as a safe value */
        return 0;

    /* Extend the cache until we have at least n entries. */
    while (true) {
#ifdef __cplusplus
        if (cacheVec.size() >= n) break;
#else
        if (cache_len >= n) break;
#endif
        /* Compute the candidate prime test. */
        seq_t candidate = (seq_t)14 * (seq_t)next_k - (seq_t)1;
        /* Check that candidate is a prime; nonnegative k are allowed. */
        if (is_prime_big(candidate)) {
#ifdef __cplusplus
            cacheVec.push_back((seq_t)next_k);
#else
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
#endif
        }
        ++next_k;
    }

    /* Return the requested term. */
#ifdef __cplusplus
    return cacheVec[n - 1];
#else
    return cache[n - 1];
#endif
}

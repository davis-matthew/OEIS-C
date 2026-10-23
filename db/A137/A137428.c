/*
 * A137428  Positive integers n which have a composite divisor smaller than
 *          their largest prime factor.
 * https://oeis.org/A137428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A137428 (Positive integers n which have a
 * composite divisor smaller than their largest prime factor) by brute‑force
 * searching through the integers in increasing order and caching the ones
 * that satisfy the property.  The function is memoised so that calls with
 * arbitrary order of n are handled efficiently.  It handles both fixed
 * integer types and arbitrary‑precision integer types for seq_t. */

/* Necessary headers */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

#if defined(__cplusplus)
#include <vector>
#endif

/* Prime list cache */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static uint64_t prime_limit = 0;

/* Ensure we have all primes up to at least ‘need’.  The array is rebuilt
 * on demand; this keeps the total number of primes small. */
static void ensure_primes(uint64_t need)
{
    if (prime_limit >= need)
        return;
    free(prime_list);
    build_primes_upto(need, &prime_list, &prime_count);
    prime_limit = need;
}

/* Test whether m satisfies the defining property of A137428. */
static bool satisfies(uint64_t m)
{
    if (m < 4)                 /* need a composite divisor */
        return false;

    uint64_t temp = m;
    uint64_t largest = 0;
    uint64_t p1 = 0, p2 = 0;

    seq_t min_comp = 0;  /* minimal composite divisor */
    bool found = false;

    /* Factorise m using the prime list that covers sqrt(m) */
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            uint64_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }

            if (p1 == 0)
                p1 = p;
            else if (p2 == 0)
                p2 = p;

            if (cnt >= 2) {                 /* p² is composite */
                seq_t cand = (seq_t)p * (seq_t)p;
                if (!found || cand < min_comp) {
                    min_comp = cand;
                    found = true;
                }
            }
            if (p2 != 0) {                   /* p1·p2 is composite */
                seq_t cand = (seq_t)p1 * (seq_t)p2;
                if (!found || cand < min_comp) {
                    min_comp = cand;
                    found = true;
                }
            }
            if (p > largest)
                largest = p;
        }
    }

    if (temp > 1) {                       /* remaining prime factor > sqrt(m) */
        if (p1 == 0)
            p1 = temp;
        else if (p2 == 0)
            p2 = temp;
        if (temp > largest)
            largest = temp;
    }

    if (!found)
        return false;                     /* no composite divisor found */

    return min_comp < (seq_t)largest;
}

/* Sequence cache and state */
#if defined(__cplusplus)
/* C++ build: use std::vector */
static std::vector<seq_t> seq_cache;
#include <cstddef>
#else
/* C build: use raw array */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
#endif
static uint64_t last_checked = 1;          /* next integer to test */

/* Append a new term to the cache */
#if defined(__cplusplus)
static inline void append_cache(seq_t val)
{
    seq_cache.push_back(val);
}
#else
static inline void append_cache(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t newcap = seq_cap ? seq_cap * 2 : 64;
        void *tmp = realloc(seq_cache, newcap * sizeof(seq_t));
        seq_cache = (seq_t *)tmp;
        seq_cap = newcap;
    }
    seq_cache[seq_len++] = val;
}
#endif

/* Public function: a(idx_t n) with 1‑based indexing.
 * It will compute and cache all terms up to the requested index. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n==0 */

    while (true) {
#if defined(__cplusplus)
        if (seq_cache.size() >= (size_t)n)
            break;
#else
        if (seq_len >= (size_t)n)
            break;
#endif
        uint64_t m = last_checked++;   /* test next integer */
        uint64_t sqrt_m = (uint64_t)sqrt((double)m) + 1;
        ensure_primes(sqrt_m);
        if (satisfies(m))
            append_cache((seq_t)m);
    }

#if defined(__cplusplus)
    return seq_cache[(size_t)n - 1];
#else
    return seq_cache[(size_t)n - 1];
#endif
}

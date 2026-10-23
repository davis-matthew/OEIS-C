/*
 * A160689  a(1)=1. a(n) = the smallest positive integer such that d(a(n)) =
 *          d(Sum_{k=1..n} a(k)), where d(m) = the number of divisors of m.
 * https://oeis.org/A160689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   int_sqrt_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A160689: a(1)=1.  For n>1,
   a(n) is the smallest positive integer x such that
   d(x) = d( Σ_{k=1..n} a(k) ),  where d(m) is the divisor count of m.  */

/* Standard headers */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/int_sqrt_uint32.h"

/* ---------- integer square root (64‑bit) ---------- */
static uint64_t int_sqrt_uint64(uint64_t n) {
    uint64_t lo = 0, hi = n;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

/* ---------- prime list handling ---------- */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;
static uint32_t prime_limit = 1;

static void grow_prime_array(size_t new_cap) {
    if (new_cap <= prime_cap) return;
    size_t cap = prime_cap ? prime_cap : 8;
    while (cap < new_cap) cap <<= 1;
    uint32_t *tmp = (uint32_t *)realloc(prime_list, cap * sizeof(uint32_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap = cap;
}

static void add_prime(uint32_t p) {
    grow_prime_array(prime_count + 1);
    prime_list[prime_count++] = p;
}

static void ensure_primes_upto(uint32_t n) {
    if (prime_limit >= n) return;
    for (uint32_t cand = prime_limit + 1; cand <= n; ++cand) {
        bool is_prime = true;
        uint32_t sq = int_sqrt_uint32(cand);
        for (size_t i = 0; i < prime_count && prime_list[i] <= sq; ++i) {
            if (cand % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) add_prime(cand);
    }
    prime_limit = n;
}

/* ---------- divisor count for 64‑bit integers ---------- */
static uint32_t divisor_count_uint64(uint64_t n) {
    if (n <= 1) return 1;
    uint32_t di = 1;
    uint64_t tmp = n;
    uint32_t sq = (uint32_t)int_sqrt_uint64(tmp);
    ensure_primes_upto(sq);
    for (size_t i = 0; i < prime_count && prime_list[i] <= sq; ++i) {
        uint32_t p = prime_list[i];
        if (tmp % p == 0) {
            uint32_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            di *= (exp + 1);
            sq = (uint32_t)int_sqrt_uint64(tmp);
        }
    }
    if (tmp > 1) di *= 2;
    return di;
}

/* ---------- sequence storage ---------- */
static seq_t *seq_cache = NULL;      /* array of computed a(n) */
static size_t cache_size = 0;        /* number of terms stored */
static size_t cache_cap   = 0;        /* allocated capacity */
static uint64_t sum_cache = 0;        /* sum of stored terms */

/* Ensure capacity for at least 'need' elements */
static void ensure_cache_size(size_t need) {
    if (cache_cap >= need) return;
    size_t cap = cache_cap ? cache_cap : 8;
    while (cap < need) cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    cache_cap = cap;
}

/* ---------- required function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but return 0 for safety */

    /* Return if already computed */
    if ((idx_t)cache_size >= n) return seq_cache[n - 1];

    /* Generate terms up to n */
    for (idx_t idx = (idx_t)cache_size; idx < n; ++idx) {
        /* current sum of first idx terms is sum_cache */
        seq_t candidate = 1;
        while (true) {
            uint64_t cand64 = (uint64_t)candidate;          /* fits in 64-bit for search */
            uint32_t d_candidate = divisor_count_uint64(cand64);
            uint64_t sum_candidate = sum_cache + cand64;
            uint32_t d_sum = divisor_count_uint64(sum_candidate);
            if (d_candidate == d_sum) break;
            ++candidate;
        }
        /* Store the new term */
        ensure_cache_size(cache_size + 1);
        seq_cache[cache_size++] = candidate;
        sum_cache += (uint64_t)candidate;
    }
    return seq_cache[n - 1];
}

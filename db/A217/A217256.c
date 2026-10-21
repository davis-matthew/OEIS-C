/*
 * A217256  Triprimes that become squares when their central digit (or central
 *          pair of digits) is deleted.
 * https://oeis.org/A217256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, cmp_seq_t, digit_count, pow10,
 *            seq_alloc_new_array, seq_free, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the OEIS sequence A217256 (Triprimes that become squares when
   their central digit or central pair of digits is deleted). The function
   a(idx_t n) returns the n‑th smallest such integer. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/build_primes_upto.h" /* build_primes_upto(idx_t, uint64_t**, size_t*) */
#include "../utils/pow10.h"             /* pow10(idx_t) */
#include "../utils/digit_count.h"       /* digit_count(uint64_t) */

#include "../utils/seq_alloc_new_array.h" /* seq_alloc_new_array(size_t) */
#include "../utils/seq_realloc.h"         /* seq_realloc(seq_t*, size_t, size_t) */
#include "../utils/seq_free.h"            /* seq_free(seq_t*) */
#include "../utils/cmp_seq_t.h"           /* cmp_seq_t(const void*, const void*) */

/* Forward declaration */
static bool is_triprime(seq_t n);

/* Dynamically grow an array of seq_t using the provided seq_* helpers */
static void push_back_array(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 16;
        seq_t *tmp = seq_realloc(*arr, *cap, new_cap);
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    ++(*len);
}

/* Prime list, built on first use */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static bool primes_built = false;

/* Test whether n is a product of exactly three primes (counted with multiplicity) */
static bool is_triprime(seq_t n)
{
    if (!primes_built) {
        build_primes_upto(1000000, &prime_list, &prime_count);
        primes_built = true;
    }

    /* If n does not fit into uint64_t, reject it – we only factor 64‑bit values */
    uint64_t val = (uint64_t)n;
    if (val < 2) return false;

    int cnt = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > val) break;
        while (val % p == 0) {
            ++cnt;
            if (cnt > 3) return false;
            val /= p;
        }
    }
    if (val > 1) ++cnt;
    return cnt == 3;
}

/* Global cache holding the sorted sequence */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t next_square = 1;  /* next k such that k^2 will be processed */

/* Generate candidates from squares k^2, k from start to end-1; collect in cache */
static void generate_block(uint64_t start, uint64_t end)
{
    for (uint64_t k = start; k < end; ++k) {
        uint64_t sq = k * k;                 /* k^2 fits into 64 bits for the used range */
        int sq_digits = digit_count(sq);
        if (sq_digits % 2) continue;         /* need even number of digits */

        uint64_t half      = sq_digits / 2;
        seq_t s            = (seq_t)sq;
        seq_t pow10_half         = pow10(half);
        seq_t pow10_half_plus1   = pow10(half + 1);
        seq_t pow10_half_plus2   = pow10(half + 2);

        seq_t left  = s / pow10_half;
        seq_t right = s % pow10_half;

        /* Odd‑digit candidates: insert single digit between halves */
        for (int mid = 0; mid <= 9; ++mid) {
            seq_t cand = left * pow10_half_plus1 +
                         (seq_t)mid * pow10_half + right;
            if (is_triprime(cand)) {
                push_back_array(&cache, &cache_len, &cache_cap, cand);
            }
        }
        /* Even‑digit candidates: insert two‑digit pair between halves */
        for (int pair = 0; pair <= 99; ++pair) {
            seq_t cand = left * pow10_half_plus2 +
                         (seq_t)pair * pow10_half + right;
            if (is_triprime(cand)) {
                push_back_array(&cache, &cache_len, &cache_cap, cand);
            }
        }
    }
}

/* Sort and deduplicate the cache after each block of squares */
static void sort_and_dedup_cache(void)
{
    if (cache_len == 0) return;

    qsort(cache, cache_len, sizeof(seq_t), cmp_seq_t);

    size_t new_len = 0;
    for (size_t i = 0; i < cache_len; ++i) {
        if (i == 0 || cmp_seq_t(&cache[i-1], &cache[i]) != 0) {
            cache[new_len++] = cache[i];
        }
    }
    cache_len = new_len;
}

/* Ensure that cache has at least n terms (sorted, unique) */
static void ensure_terms(idx_t n)
{
    if ((size_t)n <= cache_len) return;

    const uint64_t BLOCK = 2000;                     /* squares processed per block */
    while ((size_t)n > cache_len) {
        uint64_t start = next_square;
        uint64_t end   = next_square + BLOCK;
        generate_block(start, end);
        next_square = end;
        sort_and_dedup_cache();
    }
}

/* Entry point for the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n < 1; return 0 */
    ensure_terms(n);
    return cache[(size_t)n - 1];
}

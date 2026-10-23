/*
 * A106882  Primes of the form 2x^2+2xy+5y^2, with x and y nonnegative.
 * https://oeis.org/A106882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1301, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_uint64, isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 2x²+2xy+5y² with x,y ≥ 0 (OEIS A106882) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrime64.h"   /* provides bool isPrime64(uint64_t) */
#include "../utils/cmp_uint64.h"

static seq_t *prime_cache = NULL;   /* array of found primes */
static size_t prime_cache_len = 0;  /* number of stored primes */
static uint64_t cache_limit = 0;    /* current upper bound used to generate */

static void build_cache(uint64_t limit)
{
    /* Gather all candidates up to 'limit'. */
    uint64_t *cand = NULL;
    size_t cand_cap = 0;
    size_t cand_len = 0;

    for (uint64_t y = 0;; ++y) {
        uint64_t y2 = y * y;
        if (5 * y2 > limit) break;
        uint64_t base = 5 * y2;              /* 5y² part */

        for (uint64_t x = 0;; ++x) {
            /* Compute value = 2x² + 2xy + 5y²  */
            uint64_t val = 2 * x * x + 2 * x * y + base;
            if (val > limit) break;
            if (val >= 2) {
                if (cand_len == cand_cap) {
                    size_t new_cap = cand_cap ? cand_cap * 2 : 1024;
                    uint64_t *tmp = (uint64_t *)realloc(cand, new_cap * sizeof(uint64_t));
                    if (!tmp) { free(cand); return; }
                    cand = tmp;
                    cand_cap = new_cap;
                }
                cand[cand_len++] = val;
            }
        }
    }

    if (cand_len == 0) {
        free(cand);
        return;
    }

    /* Sort and deduplicate candidates. */
    qsort(cand, cand_len, sizeof(uint64_t), cmp_uint64);
    size_t uniq_len = 0;
    for (size_t i = 0; i < cand_len; ++i) {
        if (uniq_len == 0 || cand[i] != cand[uniq_len - 1]) {
            cand[uniq_len++] = cand[i];
        }
    }

    /* Allocate array for primes. */
    seq_t *tmp = (seq_t *)realloc(prime_cache, uniq_len * sizeof(seq_t));
    if (!tmp) {
        free(cand);
        return;
    }
    prime_cache = tmp;
    prime_cache_len = 0;

    for (size_t i = 0; i < uniq_len; ++i) {
        uint64_t v = cand[i];
        if (isPrime64(v)) {
            prime_cache[prime_cache_len++] = (seq_t)v;
        }
    }

    /* Shrink to the number of primes found. */
    tmp = (seq_t *)realloc(prime_cache, prime_cache_len * sizeof(seq_t));
    if (tmp) prime_cache = tmp;

    free(cand);
    cache_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1 */

    static bool initialized = false;
    if (!initialized) {
        cache_limit = 2000000ULL;   /* start with 2 million, adjust as needed */
        build_cache(cache_limit);
        initialized = true;
    }

    /* If we don't have enough primes yet, extend the bound exponentially. */
    while (prime_cache_len < (size_t)n) {
        cache_limit *= 10;
        /* Rebuild from scratch to keep the code simple and correct. */
        free(prime_cache);
        prime_cache = NULL;
        prime_cache_len = 0;
        build_cache(cache_limit);
    }

    return prime_cache[n - 1];
}

/*
 * A118955  Numbers of the form 2^k + prime.
 * https://oeis.org/A118955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 181, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: numbers of the form 2^k + prime, using a growing sieve approach. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"

static idx_t primes_limit = 0;
static uint64_t *primes = NULL;
static size_t    primes_cnt = 0;

static idx_t visited_limit = 0;
static uint8_t *visited = NULL;

static seq_t *terms = NULL;
static size_t  terms_cap   = 0;
static size_t  term_count  = 0;
static idx_t   current_limit = 0;  /* highest index processed in visited */

static void ensure_primes_upto(idx_t limit)
{
    if (limit <= primes_limit) return;
    uint64_t *new_primes;
    size_t   new_cnt;
    build_primes_upto(limit, &new_primes, &new_cnt);
    free(primes);
    primes    = new_primes;
    primes_cnt = new_cnt;
    primes_limit = limit;
}

static void ensure_terms(idx_t n)
{
    while (term_count < n) {
        /* Grow the search limit roughly twice. */
        idx_t new_limit = current_limit * 2 + 1;
        if (new_limit == 0) new_limit = 1;

        /* Expand the visited bitmap if needed. */
        if (new_limit > visited_limit) {
            visited = (uint8_t *)realloc(visited,
                                         (size_t)(new_limit + 1) * sizeof(uint8_t));
            memset(visited + visited_limit + 1, 0,
                   (size_t)(new_limit - visited_limit) * sizeof(uint8_t));
            visited_limit = new_limit;
        }

        /* Prepare a prime list up to the new limit. */
        ensure_primes_upto(new_limit);

        /* Mark every 2^k + p (p prime) within the range. */
        for (int k = 0; k < 63; ++k) {
            idx_t base = (idx_t)1 << k;
            if (base > new_limit) break;
            idx_t max_p = new_limit - base;
            for (size_t i = 0; i < primes_cnt && primes[i] <= max_p; ++i) {
                idx_t val = base + (idx_t)primes[i];
                if (val <= new_limit)
                    visited[val] = 1;
            }
        }

        /* Append newly found terms in order. */
        idx_t start = current_limit + 1;
        if (start == 0) start = 1;          /* skip index 0 */
        for (idx_t i = start; i <= new_limit; ++i) {
            if (visited[i]) {
                if (term_count == terms_cap) {
                    size_t new_cap = terms_cap ? terms_cap * 2 : 4;
                    terms = (seq_t *)realloc(terms,
                                             new_cap * sizeof(seq_t));
                    terms_cap = new_cap;
                }
                terms[term_count++] = (seq_t)i;
            }
        }

        current_limit = new_limit;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, guard anyway */
    ensure_terms(n);
    return terms[n - 1];
}

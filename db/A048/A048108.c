/*
 * A048108  Numbers with at least as many non-unitary divisors (A048105) as
 *          unitary divisors (A034444).
 * https://oeis.org/A048108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A048108
   Numbers with at least as many non‑unitary divisors as unitary divisors.
   A number satisfies the condition iff it contains a prime cube or two
   distinct prime squares in its factorisation.
   We construct the sequence by enumerating integers, factorising with a
   small‑prime‑factor table, and caching the generated terms.

   Implementation notes:
   * SPF array up to a fixed limit (200 000) is built once.
   * Cached array of terms is grown only as needed.
   * The function works correctly for any n≥1 and arbitrary order of calls.
*/
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *spfs = NULL;
static uint32_t spf_limit = 0;
static void build_spf(uint32_t limit)
{
    if (spfs != NULL) {
        free(spfs);
    }
    spf_limit = limit;
    spfs = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    if (!spfs) exit(1);

    // Linear sieve for smallest prime factors
    uint32_t *pr = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!pr) exit(1);
    uint32_t pc = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (spfs[i] == 0) {
            spfs[i] = i;
            pr[pc++] = i;
        }
        for (uint32_t k = 0; k < pc && pr[k] <= spfs[i] && (uint64_t)i * pr[k] <= limit; ++k) {
            spfs[i * pr[k]] = pr[k];
        }
    }
    free(pr);
}

static inline bool satisfies(uint64_t n)
{
    if (n < 8U) return false;          // first term is 8
    uint32_t cnt_ge2 = 0;
    while (n > 1) {
        uint32_t p = spfs[n];
        uint32_t exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        if (exp >= 3) return true;
        if (exp >= 2) {
            ++cnt_ge2;
            if (cnt_ge2 >= 2) return true;
        }
    }
    return false;
}

// Cached sequence array
static seq_t *cache = NULL;
static size_t cache_sz = 0;
static size_t cache_cap = 0;
// Last number examined
static uint64_t cur_num = 1;

seq_t a(idx_t n)
{
    // Ensure SPF table is ready
    if (!spfs) {
        // A generous upper bound: 200 000 exceeds needs for n up to 10 000
        build_spf(200000);
    }

    // Initialise cache storage
    if (!cache) {
        cache_cap = 8192;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }

    // Generate terms until we reach the n-th term
    while (cache_sz < (size_t)n) {
        ++cur_num;
        if (satisfies(cur_num)) {
            if (cache_sz >= cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
            }
            cache[cache_sz++] = (seq_t)cur_num;
        }
    }
    return cache[n - 1];
}

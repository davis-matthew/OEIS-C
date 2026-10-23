/*
 * A137257  A number k is included if there is at least one (nonzero) exponent
 *          in the prime factorization of k that is not coprime to k.
 * https://oeis.org/A137257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137257: numbers k for which there exists an exponent e in
 * the prime factorization of k with gcd(e,k)>1.  This function
 * lazily generates the sequence by scanning integers, factorizing
 * each, and caching the qualifying numbers so that a(n) can be
 * returned for any n ≥ 1 in any order of calls. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64 */

static inline uint64_t ugcd(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

static bool has_bad_exponent(uint64_t k) {
    if (k < 2) return false;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i)
        if (ugcd(k, exps[i]) > 1) return true;
    return false;
}

static seq_t *cache   = NULL;     /* cached sequence values          */
static size_t cache_cap = 0;      /* capacity of the malloc'd array */
static size_t cache_n   = 0;      /* number of terms currently cached */
static idx_t  last_checked = 1;   /* last integer examined          */

seq_t a(idx_t n) {
    if (n == 0) {                 /* offset is 1; undefined below 1 */
        abort();
    }
    /* Grow cache until at least n terms have been found. */
    while (cache_n < (size_t)n) {
        ++last_checked;
        if (has_bad_exponent((uint64_t)last_checked)) {
            if (cache_n == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_n++] = (seq_t)last_checked;
        }
    }
    return cache[(size_t)n - 1];
}

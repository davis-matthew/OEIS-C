/*
 * A093866  a(0)=1, then a(n) is the least number such that there are exactly
 *          n numbers coprime to a(n-1) between a(n-1) and a(n) (excluded).
 * https://oeis.org/A093866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, seq_alloc_new_array,
 *            seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A093866: a(0)=1; for n>0, a(n) is the least integer x > a(n-1)
 * such that exactly n integers coprime to a(n-1) lie strictly between
 * a(n-1) and x.  The implementation memoises all computed terms and
 * uses binary search with inclusion–exclusion over the prime factors of
 * a(n‑1) to find each next term efficiently.  All helper functions
 * used are provided by the build system (e.g. _floor_isqrt_u64, 
 * build_primes_upto, seq_alloc_new_array, seq_free_array).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* memoised cache of computed a(n) values */
static seq_t *cache     = NULL;  /* array of values */
static size_t cache_len = 0;     /* number of values stored   */
static size_t cache_cap = 0;     /* allocated capacity        */

/* dynamic prime list for factorisation */
static uint64_t *prime_list = NULL;
static size_t     prime_cnt  = 0;

/* Ensure cache can hold at least 'need' entries. */
static void grow_cache(size_t need) {
    if (need <= cache_cap) return;
    if (cache_cap == 0) cache_cap = 64;
    while (cache_cap < need) cache_cap <<= 1;
    seq_t *new_cache = seq_alloc_new_array(cache_cap);
    if (cache != NULL) {
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        seq_free_array(cache);
    }
    cache = new_cache;
}

/* Ensure we have all primes up to 'limit' in prime_list. */
static void ensure_primes_up_to(uint64_t limit) {
    if (prime_cnt > 0 && prime_list[prime_cnt - 1] >= limit) return;
    uint64_t *new_list = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_list, &new_cnt);
    free(prime_list);
    prime_list = new_list;
    prime_cnt  = new_cnt;
}

/* Extract distinct prime factors of n (assumed to fit in uint64_t). */
static void factor_distinct(seq_t n, uint64_t *p, size_t *cnt) {
    uint64_t m = (uint64_t)n;                /* fit assumption is safe up to n ~ 1e19 */
    uint64_t lim = _floor_isqrt_u64(m);
    ensure_primes_up_to(lim);
    *cnt = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= lim; ++i) {
        uint64_t pr = prime_list[i];
        if (m % pr == 0) {
            p[(*cnt)++] = pr;
            while (m % pr == 0) m /= pr;
        }
    }
    if (m > 1) p[(*cnt)++] = m;   /* remaining prime > sqrt */
}

/* Count numbers 1 <= k <= t coprime to a(n-1), given its distinct primes. */
static seq_t coprime_count(const uint64_t *pr, size_t r, uint64_t t) {
    seq_t cnt = (seq_t)t;            /* all numbers */
    for (uint64_t mask = 1; mask < (1ULL << r); ++mask) {
        uint64_t prod = 1;
        int bits = 0;
        for (size_t j = 0; j < r; ++j)
            if (mask & (1ULL << j)) {
                prod *= pr[j];
                ++bits;
            }
        seq_t term = (seq_t)(t / prod);
        if (bits & 1) cnt -= term;
        else          cnt += term;
    }
    return cnt;
}

/* ------------------------------------------------------------------ */
/*  Main function required by the exercise.                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;  /* a(0) = 1 */

    /* Ensure cache large enough for computation up to n. */
    grow_cache((size_t)n + 1);

    /* Initialise first term if necessary */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Return already computed value */
    if ((idx_t)cache_len > n) return cache[(size_t)n];

    /* Compute missing terms up to n */
    uint64_t prime_factors[20];
    size_t pf_cnt = 0;
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t prev = cache[i - 1];

        /* Factorise prev */
        factor_distinct(prev, prime_factors, &pf_cnt);

        /* Binary search for minimal d such that
           count of coprime numbers in (prev, prev+d) equals i.   */
        uint64_t t_low  = 0;           /* t = d-1, initial 0 */
        uint64_t t_high = 1;
        while (coprime_count(prime_factors, pf_cnt, t_high) < (seq_t)i)
            t_high <<= 1;
        while (t_low < t_high) {
            uint64_t mid = (t_low + t_high) >> 1;
            if (coprime_count(prime_factors, pf_cnt, mid) >= (seq_t)i)
                t_high = mid;
            else
                t_low = mid + 1;
        }
        uint64_t d = t_low + 1;         /* d = t_low + 1 */
        cache[i] = prev + (seq_t)d;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}

/*
 * A102884  a(n) * 6 does not appear in A102883.
 * https://oeis.org/A102884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   integer_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th positive integer x such that g(6*x) = 1,
   where g(m) = gcd(m, m + Σ_{d|m, d≤√m} d).
   The sequence consists of the x’s whose multiple of 6 is excluded
   from A102883. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/integer_sqrt.h"

/* cache for already computed a(i) values (stored as uint64_t) */
static idx_t *cache = NULL;       /* holds the true values of a(i) */
static idx_t cache_len = 0;       /* number of filled entries            */
static idx_t cache_cap = 0;       /* allocated capacity                  */
static idx_t next_candidate = 1;  /* next x to test                      */

/* Euclidean GCD for 64‑bit integers */
static inline idx_t gcd_u64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* true if 6*x is excluded from A102883 */
static inline bool excluded_by_rule(idx_t x)
{
    idx_t m = x * 6;
    idx_t sqrt_m = integer_sqrt(m);
    idx_t sum_div = 0;
    for (idx_t d = 1; d <= sqrt_m; ++d) {
        if (m % d == 0)
            sum_div += d;
    }
    idx_t s = m + sum_div;
    return gcd_u64(m, s) == 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but safe to return 0 */

    while (cache_len < n) {
        /* grow cache if necessary */
        if (cache_cap <= cache_len) {
            idx_t new_cap = cache_cap ? cache_cap << 1 : 64;
            idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
            if (!tmp) abort();             /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* find next x satisfying the exclusion rule */
        while (true) {
            if (excluded_by_rule(next_candidate)) {
                cache[cache_len++] = next_candidate;
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }
    }

    return (seq_t)cache[n - 1];
}

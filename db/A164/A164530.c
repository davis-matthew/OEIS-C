/*
 * A164530  a(1)=1. a(n) = the smallest multiple of d(n) that is > a(n-1),
 *          where d(n) = the number of positive divisors of n.
 * https://oeis.org/A164530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the smallest multiple of d(n) that is > a(n-1), where
 *            d(n) = number of positive divisors of n, and a(1) = 1
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function may be called with indices in any order;
   it caches previously computed values for efficiency. */

#include <stdlib.h>
#include <stdbool.h>

/* --- Helper: divisor count ------------------------------------------- */
static seq_t
divisor_count(idx_t n)
{
    if (n == 0) return 0;          /* defensive, should never happen */

    seq_t result = 1;
    idx_t m = n;
    idx_t p = 2;

    while (p * p <= m) {
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if (exp > 0) {
            result *= (seq_t)(exp + 1);
        }
        ++p;
    }
    if (m > 1) {                   /* remaining prime factor */
        result *= (seq_t)2;
    }
    return result;
}

/* --- Main sequence function ------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for 0 */

    /* Static cache for computed terms */
    static seq_t *cache = NULL;    /* indices 0 .. max_computed */
    static idx_t  max_computed = 0;
    static size_t cache_cap = 0;

    /* Return cached value if already computed */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Grow the cache if necessary */
    size_t needed_cap = (size_t)n + 1;   /* include index 0 */
    if (cache_cap < needed_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : needed_cap;
        if (new_cap < needed_cap) new_cap = needed_cap;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        if (k == 1) {
            cache[1] = (seq_t)1;
        } else {
            seq_t d     = divisor_count(k);
            seq_t prev  = cache[k - 1];
            seq_t m     = prev / d;
            ++m;
            cache[k]    = m * d;
        }
    }
    max_computed = n;

    return cache[n];
}

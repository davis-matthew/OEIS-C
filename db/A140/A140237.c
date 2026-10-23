/*
 * A140237  A number n is included if (Sum_{k=1..n} d(k)) is divisible by
 *          d(n), where d(n) is the number of positive divisors of n.
 * https://oeis.org/A140237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive integer m such that Sum_{k=1..m} d(k)
 *            is divisible by d(m), where d(k) is the number of positive
 *            divisors of k
 * Range:     u8 n <= 16, u16 n <= 1507, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* count divisors of m by trial division up to sqrt(m) */
static seq_t _divisor_count(idx_t m)
{
    seq_t cnt = 0;
    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            cnt++;                /* divisor i */
            if (i * i != m)       /* divisor m/i, distinct from i */
                cnt++;
        }
    }
    return cnt;
}

/* Returns the n-th element of A140237.  The sequence is generated
   incrementally and cached so that calls in arbitrary order are
   efficient. */
seq_t a(idx_t n)
{
    /* static state to allow incremental generation and caching */
    static idx_t      current = 0;   /* last processed integer */
    static seq_t      sum     = 0;   /* cumulative sum of d(k) up to 'current' */
    static seq_t     *cache   = NULL;/* cache of accepted numbers */
    static size_t    cache_len = 0;  /* number of cached elements */
    static size_t    cache_cap = 0;  /* allocated capacity of cache */

    /* extend cache until it contains at least n elements */
    while (cache_len < n) {
        /* move to next integer */
        ++current;

        /* divisor count of current */
        seq_t d = _divisor_count(current);

        /* update cumulative sum */
        sum += d;

        /* test divisibility condition */
        if (sum % d == 0) {
            /* store accepted integer in cache */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) exit(1);  /* simple failure handling */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = current;
        }
    }

    /* return the requested element (1‑based index) */
    return cache[n - 1];
}

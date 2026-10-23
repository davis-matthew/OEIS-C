/*
 * A135731  a(1) = 3; thereafter a(n+1) = a(n) + nextprime(a(n)) -
 *          prevprime(a(n)).
 * https://oeis.org/A135731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 3; thereafter a(n+1) = a(n) + nextprime(a(n)) -
 *            prevprime(a(n))
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/prev_prime.h"

/* External function required by the build system */
seq_t a(idx_t n)
{
    /* Cache to store previously computed terms so that calls in any order
       are answered in O(1) for obtained values, and new values are
       generated in a single forward pass. */
    static seq_t *cache        = NULL;
    static size_t   cache_cap  = 0;
    static idx_t    cache_len  = 0;
    static int      initialized = 0;

    /* Initialise cache on first call */
    if (!initialized) {
        cache_cap = 4;
        /* cast malloc to seq_t* for C++ compatibility */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                /* out of memory – caller will see 0 */
        cache[0] = (seq_t)3;                /* a(1) */
        cache_len = 1;
        initialized = 1;
    }

    if (n == 0) return 0; /* behaviour for indices below the offset is irrelevant */

    /* Return already cached value if available */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow the cache if necessary */
    if ((idx_t)cache_cap < n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap < n)
            new_cap <<= 1;                /* double until it fits */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* out of memory – caller will see 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Continue the recurrence from the last stored value up to n */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t cur = cache[i - 1];
        idx_t cur64 = (idx_t)cur;         /* cur is guaranteed to fit in idx_t for the supported n */
        idx_t prev = prev_prime(cur64);
        idx_t next = next_prime(cur64);
        seq_t diff = (seq_t)(next - prev);
        cache[i] = cur + diff;
    }

    cache_len = n;
    return cache[n - 1];
}

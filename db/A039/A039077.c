/*
 * A039077  Numbers whose base-9 representation has the same number of 0's and
 *          1's.
 * https://oeis.org/A039077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer whose base‑9 representation has an equal
 *            number of 0’s and 1’s. Computed by sequential search and caching
 * Range:     u8 n <= 124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t* cache      = NULL;     /* stored balanced numbers      */
static size_t cache_size = 0;        /* number of elements in cache  */
static size_t cache_cap  = 0;        /* capacity of allocated cache   */
static seq_t  cur        = 1;        /* next candidate to test       */

/* test whether the base‑9 representation of x has the same number of 0's
   and 1's.  Leading zeros are not present, so we just count the digits.  */
static bool balanced(seq_t x)
{
    uint32_t c0 = 0, c1 = 0;
    seq_t    t  = x;
    while (t > 0) {
        int d = (int)(t % 9);   /* digit 0‑8 */
        if (d == 0) ++c0;
        else if (d == 1) ++c1;
        t /= 9;
    }
    return c0 == c1;
}

seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (n != 0 && (size_t)(n - 1) < cache_size)
        return cache[(size_t)(n - 1)];

    /* Grow the cache until it contains at least n elements. */
    while (cache_size < n) {
        seq_t cand = cur;
        if (balanced(cand)) {
            if (cache_size == cache_cap) {
                /* Allocate/reallocate the cache. */
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp     = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) /* allocation failure: abort */ abort();
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_size++] = cand;
        }
        cur += 1;
    }
    return cache[(size_t)(n - 1)];
}

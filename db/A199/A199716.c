/*
 * A199716  Numbers k such that 6k-5 and 6k-1 are both composite.
 * https://oeis.org/A199716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   numbers k such that 6k-5 and 6k-1 are composite
 * Range:     u8 n <= 5, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation searches k sequentially, memoising found values. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

static inline bool is_composite(seq_t x)
{
    /* A number is composite if it is greater than 1 and not prime. */
    return x > 1 && !is_prime_small(x);
}

seq_t a(idx_t n)
{
    /*  The OEIS offset is 1.  For n == 0 we return 0 by convention. */
    if (n == 0)
        return 0;

    /* Static cache for memoisation across calls. */
    static seq_t *cache   = NULL;   /* array of found k values           */
    static size_t cap     = 0;      /* allocated capacity (number of slots) */
    static size_t len     = 0;      /* current number of stored elements  */
    static idx_t  last_k  = 0;      /* last k that was examined           */

    /* Return the already computed value if it is cached. */
    if (n <= len)
        return cache[n - 1];

    /* Generate new values until the n‑th term is known. */
    while (len < n) {
        /* Check the next integer k. */
        ++last_k;

        seq_t m = (seq_t)6 * last_k - 5;
        seq_t p = (seq_t)6 * last_k - 1;

        if (is_composite(m) && is_composite(p)) {
            /* Grow the cache if necessary.  For a new allocation, set a minimal
               initial capacity.  realloc accepts a NULL pointer as a request
               for allocation, so we can use it safely for both resizing and
               creating the cache. */
            if (len == cap) {
                cap = (cap == 0) ? 16 : cap * 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            /* Store the valid k. */
            cache[len++] = last_k;
        }
    }

    /* Return the requested term. */
    return cache[n - 1];
}

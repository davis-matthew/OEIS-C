/*
 * A083493  Multiples of 5 in which there is no common digit in successive
 *          terms.
 * https://oeis.org/A083493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the least multiple of 5 greater than a(n‑1) that * shares
 *            no decimal digit with a(n‑1)
 * Range:     u8 n <= 10, u16 n <= 29, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return a bit mask of the decimal digits that occur in x.
 * For example, x = 315 yields mask 0b101010. */
static unsigned digit_mask(seq_t x)
{
    unsigned mask = 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % (seq_t)10);
        mask |= 1u << d;
        x /= (seq_t)10;
    }
    return mask;
}

/* Return the n‑th term of A083493.  The function keeps a static
 * cache so that calls may come in any order. */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* array of computed terms          */
    static idx_t  cache_size = 0;     /* number of terms stored          */
    static size_t cache_cap   = 0;     /* allocated array capacity        */
    static unsigned last_mask = 0;     /* digit mask of the most recent term */

    /* --------------------------------------------------------------------
     * Initialise the cache for the first call.
     * -------------------------------------------------------------------- */
    if (cache_size == 0) {
        /* Allocate space for the very first term. */
        cache_cap = 32;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));

        cache[0] = (seq_t)5;
        last_mask = digit_mask(cache[0]);   /* mask of the digits in 5 */
        cache_size = 1;
    }

    /* --------------------------------------------------------------------
     * If the requested index is already cached, return it directly.
     * -------------------------------------------------------------------- */
    if ((idx_t)n < cache_size)
        return cache[(idx_t)n];

    /* --------------------------------------------------------------------
     * Make sure the cache is large enough for all terms up to n.
     * -------------------------------------------------------------------- */
    size_t needed = (size_t)(n + 1);
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : 32;
        while (new_cap < needed) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* --------------------------------------------------------------------
     * Compute all missing terms up to index n.
     * -------------------------------------------------------------------- */
    for (idx_t i = cache_size; i <= (idx_t)n; ++i) {
        seq_t candidate = cache[i - 1];
        unsigned cand_mask;

        /* Find the next multiple of 5 that shares no digit with the
         * previous term. */
        do {
            candidate += (seq_t)5;
            cand_mask = digit_mask(candidate);
        } while (cand_mask & last_mask);

        cache[i] = candidate;
        last_mask = cand_mask;
    }

    cache_size = (idx_t)n + 1;
    return cache[(idx_t)n];
}

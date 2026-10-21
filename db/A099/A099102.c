/*
 * A099102  Natural numbers n with fewer digits than sigma(n).
 * https://oeis.org/A099102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   digit_count, digit_len, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A099102: a(n) = the nth natural number m such that
 *            the number of decimal digits of m is less than that of σ(m),
 *            where σ(m) is the sum of the divisors of m (OEIS offset 1).
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/digit_len.h"
#include "../utils/digit_count.h"

/* static cache of discovered terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1; /* the next integer to test */

/* append a term to the cache, expanding as necessary */
static void cache_add(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;               /* allocation failed – keep old cache */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1 */

    /* keep generating candidates until we have at least n terms */
    while (cache_len < n) {
        /* digits of the candidate integer */
        int d_candidate = digit_count(next_candidate);

        /* σ(n) as a seq_t value */
        seq_t sigma_val = sigma_of(next_candidate);
        int d_sigma = digit_len(sigma_val);

        /* avoid storing quantities that do not fit in seq_t */
        uint64_t truncated = (uint64_t)(seq_t)next_candidate;
        if (truncated == next_candidate && d_candidate < d_sigma)
            cache_add((seq_t)next_candidate);

        ++next_candidate;
    }

    return cache[n - 1];
}

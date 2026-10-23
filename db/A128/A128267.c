/*
 * A128267  a(0) = 1; a(n) = gcd((number of earlier terms which are coprime to
 *          n), (sum of the earlier terms which are coprime to n)).
 * https://oeis.org/A128267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(# earlier terms coprime to n, sum of those terms)
 *            a(0) = 1 The sequence is memoised so that calls may come in any
 *            order
 * Range:     u8 n <= 340, u16 n <= 1677, u32 n <= 1677, u64 n <= 1677,
 *            u128 n <= 1677, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b) */

/* Compute gcd on idx_t values.  Only used for the internal counter
   and sum which may exceed the bit‑width of seq_t. */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t tmp = b;
        b = a % b;
        a = tmp;
    }
    return a;
}

/* Cache of previously computed terms. */
static seq_t *cached_seq = NULL;   /* array of a(0)...a(m-1) */
static size_t   cached_len  = 0;   /* number of values stored   */
static size_t   cached_cap  = 0;   /* current capacity of array */

/* Grow the cache to at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cached_cap) return;
    size_t new_size = new_cap;
    if (new_size == 0) new_size = 1;
    seq_t *tmp = (seq_t *)realloc(cached_seq, new_size * sizeof(seq_t));
    cached_seq = tmp;
    cached_cap = new_size;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure we have room for at least n+1 terms. */
    grow_cache((size_t)n + 1);

    /* Return a cached value if already computed. */
    if ((size_t)n < cached_len)
        return cached_seq[n];

    /* Compute all terms from cached_len up to n. */
    for (idx_t m = cached_len; m <= n; ++m) {
        if (m == 0) {
            cached_seq[0] = (seq_t)1;
            continue;
        }

        /* Count and sum of earlier terms that are coprime to m. */
        idx_t count = 0;
        idx_t sum   = 0;
        seq_t sm = (seq_t)m;        /* cached as seq_t for gcd calls */

        for (idx_t k = 0; k < m; ++k) {
            if (gcd(cached_seq[k], sm) == (seq_t)1) {
                ++count;
                sum += (idx_t)cached_seq[k];
            }
        }

        /* a(m) is gcd(count, sum) computed with idx_t arithmetic. */
        idx_t g = gcd_idx(count, sum);
        cached_seq[(size_t)m] = (seq_t)g;
    }

    /* Update length of cached data. */
    cached_len = (size_t)n + 1;
    return cached_seq[(size_t)n];
}

/*
 * A095736  Numbers with binary weight (A000120) <= 3.
 * https://oeis.org/A095736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 697, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Notes: a(n) is the n-th non‑negative integer whose binary expansion
 * contains at most 3 ones (binary weight ≤ 3).  The function builds
 * a cache of these numbers lazily; each call extends the cache up to
 * the requested index.  The algorithm is a simple linear search,
 * checking the popcount of successive integers until the required
 * quantity is found.  This is efficient for n up to at least 10 000. */

#include <stdint.h>
#include <stdlib.h>

/* Helper for popcount on 64‑bit values, written with only
 * portable integer operations. */
static int popcount_u64(uint64_t x)
{
    int c = 0;
    while (x) {
        x &= x - 1; /* drop lowest set bit */
        ++c;
    }
    return c;
}

/* Static cache of generated terms.  The cache is grown only when
 * needed; it keeps track of the last value inspected so that calls
 * to a() do not re‑scan from zero. */
static seq_t *cache = NULL;          /* array of terms */
static idx_t cache_len = 0;          /* number of terms stored */
static idx_t cache_cap = 0;          /* allocated capacity */
static uint64_t next_val = 0;        /* next integer to test */

/* Add a term to the cache, growing the buffer if necessary. */
static void add_term_to_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        idx_t new_cap = cache_cap ? (cache_cap << 1) : 8;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not used (offset 1), but safe */

    /* Extend the cache until it contains the n-th term. */
    while (cache_len < n) {
        /* Scan successive integers until one with ≤3 bits set is
         * found.  Since popcount is inexpensive, this loop is
         * acceptable for the expected n range. */
        while (popcount_u64(next_val) > 3) {
            ++next_val;
        }
        /* Store the qualifying integer. */
        add_term_to_cache((seq_t)next_val);
        ++next_val;
    }

    return cache[n - 1];
}

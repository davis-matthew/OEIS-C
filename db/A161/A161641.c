/*
 * A161641  Positions n such that A010060(n) + A010060(n+4) = 1.
 * https://oeis.org/A161641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 172, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A161641
 *
 * The sequence consists of all non‑negative integers m for which
 * the Thue–Morse values at m and m+4 differ:
 *   A010060(m) + A010060(m+4) = 1  <=>  parity(popcount(m)) ≠ parity(popcount(m+4))
 *
 * The implementation builds the sequence lazily.
 * It keeps a static array of the values that have been computed so far
 * and continues to extend it only when a request for a larger index
 * arrives.  Each integer m is tested once, so the cost of a large
 * request after some earlier calls is O(1) and a later request merely
 * extends the table by the needed amount.
 */

#include <stdlib.h>
#include <stdint.h>

/* helper: parity of popcount for a 64‑bit integer */
static inline unsigned parity_popcount(idx_t x)
{
    /* __builtin_popcountll is available in both C and C++ */
    return (__builtin_popcountll(x) & 1u);
}

static inline int qualifies(idx_t m)
{
    /* true if parity(popcount(m)) != parity(popcount(m+4)) */
    return parity_popcount(m) ^ parity_popcount(m + 4u);
}

/* dynamic cache of already discovered terms */
static seq_t *cache = NULL;          /* array of values */
static size_t cache_len = 0;         /* number of values stored */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t next_value = 0;         /* next m to be tested */

/* grow the cache to accommodate at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t new_size = new_cap;
    seq_t *newp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!newp) {
        /* out of memory – abort the program */
        abort();
    }
    cache = newp;
    cache_cap = new_size;
}

/* compute the n-th term (n>=1) of the sequence */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* not used – offset is 1 */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (qualifies(next_value)) {
            /* ensure space for the new element */
            if (cache_len == cache_cap)
                grow_cache(cache_cap == 0 ? 16 : 2 * cache_cap);

            /* store the discovered term; cast to seq_t to avoid warnings */
            cache[cache_len++] = (seq_t)next_value;
        }
        ++next_value;
    }

    /* return the n‑th term (1‑based) */
    return cache[n - 1];
}

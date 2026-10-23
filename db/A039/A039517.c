/*
 * A039517  Numbers whose base-12 representation has the same nonzero number
 *          of 2's and 6's.
 * https://oeis.org/A039517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A039517: numbers whose base‑12 representation has
   the same non‑zero count of digits 2 and 6.  The implementation
   searches integers sequentially and caches the results so that
   calls in arbitrary order are efficient.  It works for any seq_t
   type that supports the usual arithmetic operators. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cache of found terms.  cache[i] holds a(i+1). */
static seq_t *cache      = NULL;   /* pointer to allocated array */
static size_t cache_size = 0;      /* number of terms currently stored */
static size_t cache_cap  = 0;      /* allocated capacity of the cache     */
static seq_t last_val    = 0;      /* largest integer examined so far      */

/* Ensure that the cache can hold at least `need` entries.  It grows
   by doubling until the capacity is sufficient, and uses realloc for
   the actual memory operation. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    if (!cache) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }
    cache_cap = new_cap;
}

/* Return true iff `val` has the same positive number of digits
   2 and 6 in its base‑12 expansion.  The loop uses only integer
   operators on seq_t, which is required for arbitrary‑precision
   builds. */
static bool has_equal_2_and_6(seq_t val)
{
    seq_t v = val;
    int c2 = 0, c6 = 0;
    while (v != 0) {
        seq_t d = v % 12;
        if (d == 2) ++c2;
        else if (d == 6) ++c6;
        v = v / 12;
    }
    return (c2 == c6) && (c2 > 0);
}

/* Return the nth term of the sequence (1‑based).  If the result
   is already cached, it is returned immediately; otherwise the
   cache is extended by searching successive integers until
   the required number of terms is found. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined but safe */

    /* Return cached value if available. */
    if ((size_t)n <= cache_size) return cache[n - 1];

    /* Grow the cache sufficiently. */
    grow_cache((size_t)n);

    /* Start searching from the first integer after the largest
       value examined so far. */
    seq_t v = last_val + 1;

    /* Find terms until we have n of them. */
    while (cache_size < (size_t)n) {
        if (has_equal_2_and_6(v)) {
            cache[cache_size++] = v;
        }
        ++v;
    }

    last_val = v - 1;
    return cache[n - 1];
}

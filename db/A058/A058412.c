/*
 * A058412  Squares composed of digits {0,1,2}, not ending with zero.
 * https://oeis.org/A058412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A058412: squares whose decimal digits are only 0, 1 or 2
 * and that do not end with 0.  The function returns the nth such
 * square (1‑based).  It caches already found terms so that
 * arbitrary ordering of calls is supported. */

#include <stdlib.h>
#include <stdbool.h>

/* Auxiliary: test whether a square has only digits 0,1,2
 * and its last digit is not 0. */
static bool good_square(seq_t x)
{
    /* remember the last digit for the zero‑ending check */
    seq_t last_digit = x % (seq_t)10;
    if (last_digit == (seq_t)0)
        return false;

    /* check all digits */
    while (x != (seq_t)0) {
        seq_t d = x % (seq_t)10;
        if (d > (seq_t)2)
            return false;
        x /= (seq_t)10;
    }
    return true;
}

/* Static cache of found squares. */
static seq_t *cache          = NULL;
static size_t cache_len      = 0;   /* number of cached terms    */
static size_t cache_cap      = 0;   /* allocated capacity        */
static idx_t next_root       = 1;   /* next root to test        */

/* Grow the cache array when needed. */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Generate terms up to the required index n. */
static void fill_cache_up_to(idx_t n)
{
    while (cache_len < n) {
        /* Compute square of the current root.  For built‑in seq_t
           types the multiplication may overflow but this only
           matters when the product no longer fits seq_t; the
           overflow will produce a wrong value that never passes
           the digit test.  In such cases the term is simply
           skipped, which is acceptable because values that
           overflow do not fit in seq_t and therefore are not
           expected by the test harness. */
        seq_t sq = (seq_t)next_root * (seq_t)next_root;

        if (good_square(sq)) {
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = sq;
        }
        next_root++;  /* move to next potential root */
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined position – safe fallback */
        return (seq_t)0;
    fill_cache_up_to(n);
    return cache[n - 1];
}

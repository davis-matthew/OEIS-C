/*
 * A095779  Values of n for which A095777(n) is 10 (those terms which are
 *          expressible in decimal digits for bases 2 through 11, but not for
 *          base 12).
 * https://oeis.org/A095779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A095779 – values of n such that
 *  (1) the representation of n in base‑12 contains at least one digit 10 or 11,
 *  (2) the representation of n in base‑11 contains no digit 10.
 *  This property is equivalent to “A095777(n) = 10”.
 *
 *  Algorithm:  On demand, search integers in increasing order, checking the
 *  two base conditions.  Cache the found terms so repeated calls are fast.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_ok.h"

/* ----------- caching the computed terms ----------- */

static seq_t *cache      = NULL;   /* array of found terms                 */
static size_t cache_len  = 0;      /* number of terms in the cache         */
static size_t cache_cap  = 0;      /* allocated capacity (>= cache_len)    */
static seq_t next_tested = 1;      /* next integer candidate to test       */

/* Ensure the cache has room for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) {
        new_cap <<= 1;
    }
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* In a real program you might handle this error more gracefully. */
        exit(1);
    }
    cache        = new_ptr;
    cache_cap    = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; undefined for n == 0, but return 0 */
        return 0;

    /* Compute enough terms to satisfy the request. */
    while (cache_len < (size_t)n) {
        if (is_ok(next_tested)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = next_tested;
        }
        next_tested++;   /* move to the next candidate integer */
    }

    return cache[(size_t)n - 1];
}

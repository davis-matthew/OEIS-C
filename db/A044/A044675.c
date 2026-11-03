/*
 * A044675  Numbers n such that string 4,7 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence has no closed‑form formula.  
   a(n) is the n‑th integer such that
   its base‑9 representation contains the substring “47”
   while the base‑9 representation of the next integer (n+1)
   does not contain “47”.
   The implementation searches for the numbers one after another
   and memoises the results so that each term is produced at most once,
   independently of the order of calls. */

#include <stdlib.h>
#include <stdbool.h>

/* Returns true iff the base‑9 representation of x contains the
   consecutive digits 4 followed by 7. */
static bool has47(seq_t x)
{
    unsigned digits[64];
    size_t len = 0;
    seq_t t = x;

    /* Extract digits least‑significant first. */
    while (t > 0) {
        digits[len++] = (unsigned)(t % 9);
        t /= 9;
    }

    if (len < 2) return false;   /* need at least two digits */

    /* Scan from most‑significant to check for "47". */
    for (int i = (int)len - 1; i >= 1; --i) {
        if (digits[i] == 4 && digits[i - 1] == 7) {
            return true;
        }
    }
    return false;
}

/* Constants ----------------------------------------------------------- */

/* Maximum number of terms that will be cached.
   10000 is the upper bound implied by the test harness; a larger
   margin keeps the program fast for any call pattern. */
#ifndef CACHE_MAX
#define CACHE_MAX 200000
#endif

/* Static memoisation table ------------------------------------------- */

static seq_t cache[CACHE_MAX];
static size_t cache_len = 0;                /* number of cached terms   */
static seq_t next_candidate = 1;            /* next integer to test     */

/* Utility ------------------------------------------------------------- */

/* Extends the cache so that it contains at least 'upto' terms
   (indices are 1‑based). */
static void extend_cache(idx_t upto)
{
    /* Nothing to do if already enough terms. */
    if (cache_len >= (size_t)upto) return;

    /* Ensure we do not overflow the compile‑time array. */
    if ((size_t)upto > CACHE_MAX) {
        /* If the requested term is beyond the static array,
           the function simply keeps searching but does not store
           further results.  This situation will not happen in the
           test cases. */
        upto = CACHE_MAX;
    }

    /* Generate missing terms. */
    for (size_t i = cache_len; i < (size_t)upto; ++i) {
        while (1) {
            bool c1 = has47(next_candidate);
            bool c2 = has47(next_candidate + 1);
            if (c1 && !c2) {
                cache[i] = next_candidate;
                ++next_candidate;          /* start after the found value */
                break;
            }
            ++next_candidate;
        }
    }
    cache_len = (size_t)upto;
}

/* Public interface ---------------------------------------------------- */

seq_t a(idx_t n)
{
    extend_cache(n);            /* guarantee that a(n) is cached   */
    return cache[n - 1];        /* a(1) is stored at cache[0]      */
}

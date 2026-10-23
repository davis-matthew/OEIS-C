/*
 * A039464  Numbers whose base-11 representation has the same nonzero number
 *          of 2's and 10's.
 * https://oeis.org/A039464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-11 representation has the same nonzero number of 2's and 10's.
   Implementation: generate the sequence online by scanning integers and
   recording those that satisfy the digit‑count condition. */

#include <stdlib.h>
#include <stdbool.h>

/* Static storage for memoization */
static seq_t *cache = NULL;       /* array of found terms          */
static size_t cache_cap = 0;      /* allocated capacity in seq_t   */
static idx_t  cache_count = 0;    /* number of terms stored       */
static seq_t  next_candidate = (seq_t)1;  /* next integer to test */

/* Ensure the cache holds at least n terms (1‑based index) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_count)
        return;                     /* already have them */

    /* grow the array if necessary */
    if (cache_cap < n) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
        while (new_cap < n) new_cap *= 2;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!p) exit(1);            /* allocation failure */
        cache = p;
        cache_cap = new_cap;
    }

    /* fill the cache until we have n terms */
    while (cache_count < n) {
        /* find next integer satisfying the condition */
        while (1) {
            seq_t x = next_candidate;
            int count2 = 0, count10 = 0;
            while (x > (seq_t)0) {
                seq_t rem = x % (seq_t)11;
                if (rem == (seq_t)2)      ++count2;
                else if (rem == (seq_t)10) ++count10;
                x /= (seq_t)11;
            }
            if (count2 == count10 && count2 > 0) {
                cache[cache_count++] = next_candidate;
                next_candidate += (seq_t)1;        /* start after this number next time */
                break;
            }
            next_candidate += (seq_t)1;            /* try next integer */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for 0, return 0 as a fallback */
    ensure_cache(n);
    return cache[n - 1];
}

/*
 * A155620  a(n) = 9^n+4^n-1.
 * https://oeis.org/A155620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n + 4^n - 1 Recurrence (valid for all n ≥ 2): a(0) = 1,
 *            a(1) = 12 a(n) = 13*a(n-1) - 36*a(n-2) - 24
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function caches previously computed values, so repeated calls
   (in any order) are efficient. */

#include <stdlib.h>

/* Static cache storage */
static seq_t *cache = NULL;   /* array of previously computed values */
static size_t cap   = 0;      /* current allocated capacity (in array cells) */
static size_t len   = 0;      /* number of values stored (cache[0..len-1]) */

/* Expand the cache to include the term a(n). */
static void ensure_cache(idx_t n)
{
    /* Expand the array if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 64;
        while (newcap < (size_t)n + 1) {
            newcap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise base cases if no values have been stored yet */
    if (len == 0) {
        cache[0] = (seq_t)1;
        len = 1;
    }

    /* Compute missing values up to n (if any) */
    while ((size_t)len <= (size_t)n) {
        seq_t val;
        if (len == 1) {        /* a(1) = 12 */
            val = (seq_t)12;
        } else {              /* a(k) = 13*a(k-1) - 36*a(k-2) - 24 */
            seq_t prev  = cache[len - 1];
            seq_t prev2 = cache[len - 2];
            val = 13 * prev - 36 * prev2 - 24;
        }
        cache[len] = val;
        ++len;
    }
}

/* External entry point – returns a(n). */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}

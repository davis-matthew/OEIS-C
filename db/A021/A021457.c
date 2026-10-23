/*
 * A021457  Decimal expansion of 1/453.
 * https://oeis.org/A021457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* decimal digits of 1/453: a(0) = integer part (0), a(n) for n>=1
   gives the n‑th digit after the decimal point.  The sequence is
   computed by simulating long division with a static cache that
   remembers the current remainder. */
#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;          /* cache of already computed digits   */
static idx_t  cache_cap = 0;         /* allocated capacity (in slots)      */
static idx_t  cache_len = 0;         /* number of digits currently stored */
static idx_t  cur_rem   = 10;        /* current remainder before next digit
                                      * (starts at 10 to produce the first
                                      *  decimal digit 0 correctly)        */

/* Ensure the cache can hold indices up to n (inclusive).  */
static void ensure_capacity(idx_t n)
{
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap * 2ULL : 1ULL;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Initialise the cache on first use */
    if (cache_len == 0) {
        ensure_capacity(0);
        cache[0] = (seq_t)0;   /* integer part of 1/453 */
        cache_len = 1;
    }

    /* If the requested index is not yet cached, extend the cache. */
    if (n >= cache_len) {
        ensure_capacity(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            /* Long‑division step: multiply remainder by 10, divide by 453. */
            idx_t mult = cur_rem * 10ULL;
            seq_t digit = (seq_t)(mult / 453ULL);
            cur_rem = (idx_t)(mult % 453ULL);
            cache[i] = digit;
        }
        cache_len = n + 1;
    }

    return cache[n];
}

/*
 * A134679  a(1) = ... = a(6) = 1; a(n)=a(n-a(n-1))+a(n-a(n-6)).
 * https://oeis.org/A134679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 413, u16 n <= 2354, u32 n <= 2354, u64 n <= 2354,
 *            u128 n <= 2354, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A134679
   a(1) = a(2) = a(3) = a(4) = a(5) = a(6) = 1
   a(n) = a(n-a(n-1)) + a(n-a(n-6))    (n ≥ 7)
   The sequence is defined only up to n = 2354; for larger n this
   implementation returns 0.                                     */

#include <stdint.h>
#include <stdlib.h>

/* Static memoisation table (1‑based indexing; table[0] unused).  */
static seq_t *cache = NULL;    /* Array of computed a(n) values.   */
static size_t cache_len = 0;   /* Highest index that has been computed. */
static size_t cache_cap = 0;   /* Current allocation size (elements).   */

/* Ensure that the cache can hold indices up to 'need' (inclusive). */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Public function required by the harness.  */
seq_t a(idx_t n)
{
    /* Sequence is only defined up to 2354.  */
    if (n == 0)
        return (seq_t)0;
    if (n > (idx_t)2354)
        return (seq_t)0;

    /* Compute terms up to n if not already done.  */
    if ((idx_t)cache_len < n) {
        /* Make sure we can index up to n (array indices 0..n).  */
        ensure_capacity((size_t)n + 1);

        /* Initialise missing entries.  */
        for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i) {
            if (i <= 6) {
                cache[i] = (seq_t)1;
            } else {
                /* Compute a(i-a(i-1)) */
                idx_t sub1 = (idx_t)cache[i - 1];
                idx_t idx1 = i - sub1;

                /* Compute a(i-a(i-6)) */
                idx_t sub6 = (idx_t)cache[i - 6];
                idx_t idx2 = i - sub6;

                cache[i] = cache[idx1] + cache[idx2];
            }
        }
        cache_len = (size_t)n;
    }
    return cache[(size_t)n];
}

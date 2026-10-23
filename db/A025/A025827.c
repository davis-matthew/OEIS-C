/*
 * A025827  Expansion of 1/((1-x^2)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A025827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 11, and 12 (also
 *            satisfies the recurrence a(n) =
 *            a(n-2)+a(n-11)+a(n-12)-a(n-13)-a(n-14)-a(n-23)+a(n-25), with
 *            a(0)=1, a(1)=0, a(2)=1, a(3)=0, a(4)=1, a(5)=0, a(6)=1, …)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard header for realloc */
#include <stdlib.h>

/* Static cache for memoised values */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Return the n‑th term of the sequence */
seq_t a(idx_t n)
{
    /* If cache already covers n, just return it */
    if (n < cache_len)
        return cache[n];

    /* Need to extend cache up to index n */
    idx_t old_len = cache_len;
    idx_t new_len = n + 1;

    /* Reallocate memory */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        /* On allocation failure return 0 (unlikely in typical tests) */
        return (seq_t)0;
    }
    cache = tmp;

    /* Zero the new portion of the array */
    for (idx_t i = old_len; i < new_len; ++i)
        cache[i] = (seq_t)0;

    /* Ensure base case a(0)=1 */
    if (old_len == 0)
        cache[0] = (seq_t)1;

    /* Compute values from old_len up to n */
    for (idx_t i = old_len; i <= n; ++i) {
        /* Skip a(0) if it just got initialized */
        if (i == 0)
            continue;

        seq_t v = (seq_t)0;
        if (i >= 2)  v += cache[i - 2];
        if (i >= 11) v += cache[i - 11];
        if (i >= 12) v += cache[i - 12];
        if (i >= 13) v -= cache[i - 13];
        if (i >= 14) v -= cache[i - 14];
        if (i >= 23) v -= cache[i - 23];
        if (i >= 25) v += cache[i - 25];
        cache[i] = v;
    }

    cache_len = new_len;
    return cache[n];
}

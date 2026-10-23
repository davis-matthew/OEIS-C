/*
 * A022864  a(n) = [ a(n-1)/a(1) ] + [ a(n-2)/a(2) ] + ... + [ a(1)/a(n-1) ],
 *          for n >= 3.
 * https://oeis.org/A022864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [a(n-1)/a(1)] + [a(n-2)/a(2)] + ... + [a(1)/a(n-1)],
 *            a(1)=1, a(2)=2. (OEIS A022864)
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Ensure the cache has room for indices up to 'need' (inclusive). */
static void ensure_cache_size(idx_t need, seq_t **cache, size_t *len)
{
    if (*len <= need) {
        size_t new_len = need + 1;            /* indices 0..need */
        seq_t *tmp = (seq_t *)realloc(*cache, new_len * sizeof(seq_t));
        *cache = tmp;
        *len   = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)      /* undefined indices are treated as 0 */
        return (seq_t)0;

    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;          /* current allocation size */
    static idx_t  max_computed = 0;        /* largest n that has been computed */

    /* Allocate (or grow) the cache to accommodate index n. */
    ensure_cache_size(n, &cache, &cache_len);

    /* Initialize the base terms if this is the first call or cache was just reallocated. */
    if (max_computed == 0) {
        cache[0] = (seq_t)0;                /* unused slot for convenience */
        cache[1] = (seq_t)1;
        if (n >= 2) {
            cache[2] = (seq_t)2;
            max_computed = 2;
        } else
            max_computed = 1;
    } else {
        /* In case the cache was reallocated after some terms had already
           been computed, ensure the base terms are still present. */
        if (max_computed < 1) {
            cache[1] = (seq_t)1;
            max_computed = 1;
        }
        if (max_computed < 2 && n >= 2) {
            cache[2] = (seq_t)2;
            max_computed = 2;
        }
    }

    /* Compute any terms missing up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k < i; ++k) {
            sum += cache[i - k] / cache[k];
        }
        cache[i] = sum;
    }

    max_computed = n;
    return cache[n];
}

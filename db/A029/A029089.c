/*
 * A029089  Expansion of 1/((1-x)*(1-x^5)*(1-x^6)*(1-x^11)).
 * https://oeis.org/A029089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-5)-a(n-7)-a(n-16)+a(n-18)+a(n-22)-a(n-23) with
 *            a(0)=1 and a(k)=0 for k<0. This recurrence follows from
 *            (1-x)(1-x⁵)(1-x⁶)(1-x¹¹)·G(x)=1, where G(x)=∑a(n)xⁿ
 * Range:     u8 n <= 68, u16 n <= 494, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;   /* values up to cache_len-1 */
static idx_t  cache_len = 0;  /* number of cached elements */

/* Compute and cache a(n) for all missing indices up to n. */
seq_t a(idx_t n)
{
    /* Ensure cache has at least one element for a(0) */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Grow cache if necessary */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cache_len = new_len;

        /* Compute new entries using the recurrence */
        for (idx_t i = old_len; i <= n; ++i) {
            seq_t pos = 0, neg = 0;
            if (i >= 1)   pos += cache[i-1];
            if (i >= 5)   pos += cache[i-5];
            if (i >= 7)   neg += cache[i-7];
            if (i >= 16)  neg += cache[i-16];
            if (i >= 18)  pos += cache[i-18];
            if (i >= 22)  pos += cache[i-22];
            if (i >= 23)  neg += cache[i-23];
            cache[i] = pos - neg;            /* pos >= neg by construction */
        }
    }

    return cache[n];
}

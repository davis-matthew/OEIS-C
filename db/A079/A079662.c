/*
 * A079662  a(n) = the number of occurrences of 1 in all compositions of n
 *          without 2's = # of occurrences of the integer k in compositions of
 *          n+k-1 without 2's (k > 2).
 * https://oeis.org/A079662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 6*a(n-3) - 5*a(n-4) + 2*a(n-5) -
 *            a(n-6) for n > 6, with initial values a(1)=1, a(2)=2, a(3)=3,
 *            a(4)=6, a(5)=13, a(6)=26
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 75, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[1..n] holds computed terms   */
static idx_t  cache_size = 0;    /* current allocated size (max n cached) */

/* Helper: ensure the cache can hold terms up to n, compute missing ones. */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n)
        return;
    /* allocate new array of size n+1 (index 0 unused) */
    seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!new_cache)
        return;          /* malloc failure – caller will get 0 if accessed */
    /* copy old values if any */
    if (cache) {
        for (idx_t i = 1; i <= cache_size; ++i)
            new_cache[i] = cache[i];
    }
    /* zero remaining entries initially */
    for (idx_t i = cache_size + 1; i <= n; ++i)
        new_cache[i] = 0;
    free(cache);
    cache = new_cache;
    /* compute new terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1)          cache[i] = (seq_t)1;
        else if (i == 2)     cache[i] = (seq_t)2;
        else if (i == 3)     cache[i] = (seq_t)3;
        else if (i == 4)     cache[i] = (seq_t)6;
        else if (i == 5)     cache[i] = (seq_t)13;
        else if (i == 6)     cache[i] = (seq_t)26;
        else { /* recurrence for i > 6 */
            cache[i] = 4*cache[i-1] - 6*cache[i-2] + 6*cache[i-3]
                       - 5*cache[i-4] + 2*cache[i-5] - cache[i-6];
        }
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* offset 1: undefined below 1 */
    ensure_cache(n);
    return cache[n];
}

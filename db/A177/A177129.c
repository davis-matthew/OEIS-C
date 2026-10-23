/*
 * A177129  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=8, k=0 and l=1.
 * https://oeis.org/A177129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+1) = sum_{p=0..n} a(p)*a(n-p) + 1 with a(0) = 1, a(1) = 8
 *            (sequence A177129)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoised sequence values   */
static idx_t cache_size = 0;         /* number of values stored    */

static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;            /* nothing to do              */

    /* allocate enough space to store values up to n */
    if (cache_size == 0) {
        /* initialise with the two fixed starting values */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;
        cache[1] = 8;
        cache_size = 2;
    }

    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* compute missing values */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) { cache[0] = 1; continue; }
        if (i == 1) { cache[1] = 8; continue; }

        unsigned __int128 sum = 1;          /* the constant l = 1 */
        for (idx_t p = 0; p < i; ++p) {
            unsigned __int128 prod = (unsigned __int128)cache[p] *
                                     (unsigned __int128)cache[i - 1 - p];
            sum += prod;
        }
        cache[i] = (seq_t)sum;
    }

    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 8;
    ensure_cache(n);
    return cache[n];
}

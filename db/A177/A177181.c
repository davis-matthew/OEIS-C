/*
 * A177181  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=6, k=-1 and l=-1.
 * https://oeis.org/A177181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A177181
   Recurrence: a(0)=1, a(1)=6
   a(n+1) = sum_{p=0}^{n} a(p)*a(n-p) - (n+2)   for n >= 1
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Static cache of computed terms */
static seq_t *cache   = (seq_t *)NULL;   /* pointer to array of seq_t */
static idx_t  cache_len = 0;             /* allocated length of cache array */
static idx_t  last_computed = (idx_t)-1; /* highest index that has been computed */

/* Ensure that cache can hold entries up to index n (inclusive) */
static void
ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    idx_t new_len = cache_len ? cache_len << 1 : 2;
    while (new_len <= n)
        new_len <<= 1;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_len]();   /* default‑construct */
#else
    new_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
#endif

    if (cache) {
        /* Copy already computed terms with assignment (handles big int types) */
        for (idx_t i = 0; i <= last_computed; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache = new_cache;
    cache_len = new_len;
}

/* Return the n‑th term of A177181 */
seq_t
a(idx_t n)
{
    /* Initialise on first call */
    if (cache == NULL) {
        ensure_cache(1);              /* indices 0 and 1 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        last_computed = 1;
    }

    /* If already computed, return immediately */
    if (n <= last_computed)
        return cache[n];

    /* Grow cache if necessary */
    ensure_cache(n);

    /* Compute terms from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        idx_t m = i - 1;              /* n in the recurrence */
        seq_t sum = (seq_t)0;

        for (idx_t p = 0; p <= m; ++p) {
            sum += cache[p] * cache[m - p];
        }

        /* a(i) = sum - (m + 2) = sum - (i + 1) */
        cache[i] = sum - (seq_t)(i + 1);
    }

    last_computed = n;
    return cache[n];
}

/*
 * A121800  a(n)= 4*a(n-1) +18*a(n-2) -48*a(n-3) -60*a(n-4) +80*a(n-5)
 *          +56*a(n-6).
 * https://oeis.org/A121800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 18*a(n-2) - 48*a(n-3) - 60*a(n-4) * +
 *            80*a(n-5) + 56*a(n-6); * initial terms: a(1)=0, a(2)=159,
 *            a(3)=4694, a(4)=36506, * a(5)=190224, a(6)=1152620
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of terms stored (≥0) */

seq_t a(idx_t n)
{
    /* Ensure base values are available. */
    if (cache_size < 6) {
        /* Allocate space for indices 0..6 (index 0 unused). */
        size_t sz = (size_t)(7) * sizeof(seq_t);
        seq_t *p = (seq_t *)realloc(cache, sz);
        if (!p) return 0;     /* allocation failed */
        cache = p;
        cache_size = 6;
        cache[0] = 0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)159;
        cache[3] = (seq_t)4694;
        cache[4] = (seq_t)36506;
        cache[5] = (seq_t)190224;
        cache[6] = (seq_t)1152620;
    }

    if (n <= cache_size)
        return cache[n];

    /* Enlarge cache to hold up to n (indexing 1..n). */
    size_t new_sz = (size_t)(n + 1) * sizeof(seq_t);
    seq_t *p = (seq_t *)realloc(cache, new_sz);
    if (!p) return 0;     /* allocation failed */
    cache = p;

    /* Compute missing values using the recurrence. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        cache[i] =
            (seq_t)4   * cache[i - 1] +
            (seq_t)18  * cache[i - 2] -
            (seq_t)48  * cache[i - 3] -
            (seq_t)60  * cache[i - 4] +
            (seq_t)80  * cache[i - 5] +
            (seq_t)56  * cache[i - 6];
    }
    cache_size = n;

    return cache[n];
}

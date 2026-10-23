/*
 * A049854  a(n)=Sum{a(k): k=0,1,2,...,n-3,n-1}; a(n-2) is not a summand; 2
 *          initial terms required.
 * https://oeis.org/A049854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3); Initial values: a(0)=1,
 *            a(1)=3, a(2)=3
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoized sequence values */
static size_t capacity = 0;   /* allocated size of cache   */
static size_t last_computed = 0; /* highest index that has been computed */

/* Ensure that cache is large enough to hold indices up to 'needed' (exclusive) */
static void ensure_capacity(idx_t needed)
{
    if ((idx_t)capacity < needed) {
        size_t new_cap = capacity ? capacity * 2 : 1;
        while ((idx_t)new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = new_cap;
    }
}

/* External function requested by the framework */
seq_t a(idx_t n)
{
    /* Initialise the cache on first call */
    if (!cache) {
        capacity = 3;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)3;
        last_computed = 2;
    }

    /* Return already computed value if available */
    if (n <= last_computed)
        return cache[n];

    /* Grow cache if necessary and compute missing terms */
    ensure_capacity((idx_t)(n + 1));

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];
        val += val;              /* val = 2*a(i-1)                */
        val -= cache[i - 2];     /* val -= a(i-2)                 */
        val += cache[i - 3];     /* val += a(i-3)                 */
        cache[i] = val;
    }

    last_computed = (size_t)n;
    return cache[n];
}

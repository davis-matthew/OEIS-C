/*
 * A076644  a(1)=1; for n>1, a(n) = a(n-floor(sqrt(n))) + n.
 * https://oeis.org/A076644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n - floor(sqrt(n))) + n (a(1) = 1)
 * Range:     u8 n <= 49, u16 n <= 2095, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Integer floor square root for a 64‑bit unsigned value.
   Returns ⌊√v⌋. Simple binary search.  */
static idx_t isqrt_u64(idx_t v)
{
    idx_t low  = 0;
    idx_t high = (idx_t)1ULL << 32;     /* 2^32 > sqrt(2^64-1) */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (mid > 0 && mid > v / mid)   /* mid*mid > v */
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* Sequence A076644: a(1)=1; a(n)=a(n-floor(sqrt(n)))+n for n>1. */
seq_t a(idx_t n)
{
    if (n == 0)                /* out of range, not used by caller */
        return 0;

    /* Static cache: 1‑based, cache[i] holds a(i+1). */
    static seq_t *cache  = NULL;   /* array of a(1)...a(cached) */
    static idx_t cached  = 0;      /* number of terms already stored */

    if (n > cached) {
        /* Grow cache to accommodate up to n. */
        idx_t new_size = n; /* new number of elements */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)        /* allocation failure */
            return 0;
        cache = tmp;

        /* Compute missing terms in increasing order. */
        for (idx_t i = cached + 1; i <= n; ++i) {
            if (i == 1) {
                cache[0] = (seq_t)1;      /* a(1) */
            } else {
                idx_t k   = isqrt_u64(i);  /* floor(sqrt(i)) */
                idx_t idx = i - k;         /* >= 1 */
                cache[i-1] = cache[idx-1] + (seq_t)i;
            }
        }
        cached = n;
    }

    return cache[n-1];
}

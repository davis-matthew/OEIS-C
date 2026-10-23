/*
 * A176759  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=0, k=1 and l=-1.
 * https://oeis.org/A176759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 0, a(n+1) = (sum_{p=0}^{n} a(p)*a(n-p)) + n
 *            (for n >= 1) This implements the recurrence of OEIS A176759 with
 *            k = 1, l = -1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper to ensure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr)
{
    if (n >= *cap_ptr) {
        size_t new_cap = (*cap_ptr == 0) ? 4 : (*cap_ptr << 1);
        while (new_cap <= n) new_cap <<= 1;
        *cache_ptr = (seq_t *)realloc(*cache_ptr, new_cap * sizeof(seq_t));
        for (idx_t i = *cap_ptr; i < new_cap; ++i) (*cache_ptr)[i] = 0;
        *cap_ptr = new_cap;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static bool initialized = false;
    static idx_t cache_len = 0; /* highest index that has a computed value */

    if (!initialized) {
        /* Ensure initial capacity for indices 0 and 1 */
        ensure_cache(1, &cache, &cache_cap);
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache[1] = (seq_t)0;   /* a(1) = 0 */
        cache_len = 1;
        initialized = true;
    }

    /* Expand cache if needed */
    if (n > cache_len) {
        ensure_cache(n, &cache, &cache_cap);
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* compute a[i] using recurrence:
               a[i] = sum_{p=0}^{i-1} a[p]*a[i-1-p] + (i-1)   (since n+1 = i)
            */
            seq_t sum = 0;
            for (idx_t p = 0; p < i; ++p) {
                sum += cache[p] * cache[i - 1 - p];
            }
            sum += (seq_t)(i - 1); /* add n where n = i-1 */
            cache[i] = sum;
        }
        cache_len = n;
    }

    return cache[n];
}

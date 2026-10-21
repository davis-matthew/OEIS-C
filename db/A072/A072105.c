/*
 * A072105  Let c(1)=x, c(n+1) = c(n)/2 + n if c(n) is even, c(n+1)= 2c(n) - n
 *          otherwise; then a(n)=c(n) for c(1)=3.
 * https://oeis.org/A072105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = c(n) with recurrence: c(1) = 3 c(n+1) = c(n)/2 + n if
 *            c(n) is even = 2*c(n) - n if c(n) is odd the function gives a(n)
 *            for n ≥ 1
 * Range:     u8 n <= 40, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  // for realloc
#include <stdint.h>  // for uintptr_t if needed

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of terms stored, cache_size >= 1 if initialized */

    /* Ensure cache is large enough to hold up to n */
    if (n == 0) return (seq_t)0;  /* undefined for n=0, but guard */

    if (cache_size == 0) {
        /* First call: initialize cache with c(1)=3 */
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)3;          /* c(1) */
        cache_size = 1;
    }

    if (n > cache_size) {
        /* Extend cache to size n */
        seq_t *new_cache = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;      /* allocation failure */
        cache = new_cache;

        /* Compute missing terms from cache_size+1 up to n */
        seq_t cur = cache[cache_size - 1];   /* c(current index) */
        for (idx_t idx = cache_size + 1; idx <= n; ++idx) {
            /* idx is the target index, previous index is idx-1 */
            if ((cur & (seq_t)1) == 0)  /* even */
                cur = cur / (seq_t)2 + (seq_t)(idx - 1);
            else                       /* odd */
                cur = (seq_t)2 * cur - (seq_t)(idx - 1);
            cache[idx - 1] = cur;       /* store c(idx) */
        }
        cache_size = n;
    }

    return cache[n - 1];   /* a(n) = c(n) */
}

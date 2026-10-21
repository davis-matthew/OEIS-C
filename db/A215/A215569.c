/*
 * A215569  a(n) = 3*a(n-1) + 46*a(n-2) + a(n-3) with a(0)=0, a(1)=14,
 *          a(2)=49.
 * https://oeis.org/A215569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 46*a(n-2) + a(n-3) with a(0)=0, a(1)=14,
 *            a(2)=49
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed terms.
       cache_cap  – total allocated slots
       cache_next – next index that needs computing (initially 3, after seeds) */
    static seq_t *cache      = NULL;
    static idx_t cache_cap   = 0;
    static idx_t cache_next  = 0;   /* number of values already computed */

    /* First call: allocate space and fill initial seeds. */
    if (cache == NULL) {
        idx_t capacity = n + 1;
        if (capacity < 3) capacity = 3;          /* we always need 0..2 */
        cache = (seq_t *)malloc((size_t)capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;      /* memory error – return 0 */

        cache_cap  = capacity;
        cache_next = 3;                          /* 0..2 are known */

        /* initial values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)14;
        cache[2] = (seq_t)49;
    }

    /* Requested value already computed? */
    if (n < cache_next) {
        return cache[n];
    }

    /* Need more space?  Grow to at least n+1 slots. */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;        /* memory error – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_next; i <= n; ++i) {
        cache[i] = cache[i-1] * (seq_t)3 + cache[i-2] * (seq_t)46 + cache[i-3];
    }
    cache_next = n + 1;   /* all terms 0..n are now computed */

    return cache[n];
}

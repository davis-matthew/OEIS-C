/*
 * A136580  Row sums of triangle A136579.
 * https://oeis.org/A136580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n*a(n-1) + a(n-2) - n*a(n-3)
 * Initial values: a(0)=1, a(1)=1, a(2)=3
 * Recurrence reduces to: a(n) = n*(a(n-1)-a(n-3)) + a(n-2)
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity */
    static idx_t cache_len = 0;   /* number of values stored */

    /* Initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 4;                    /* start with room for 4 entries */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);              /* abort if allocation fails */
        cache[0] = (seq_t)1;              /* a(0) */
        cache[1] = (seq_t)1;              /* a(1) */
        cache[2] = (seq_t)3;              /* a(2) */
        cache_len = 3;                    /* we have computed up to a(2) */
    }

    if (n < cache_len) return cache[n];

    /* Extend cache until it contains a(n) */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Ensure sufficient capacity before accessing cache[i] */
        if (i >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap == 0) new_cap = 4;
            while (new_cap <= (size_t)i) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Recurrence: a(i) = i*(a(i-1)-a(i-3)) + a(i-2) */
        seq_t diff = cache[i-1] - cache[i-3];    /* a(i-1) > a(i-3) for i>=3 */
        seq_t val  = ((seq_t)i) * diff + cache[i-2];
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}

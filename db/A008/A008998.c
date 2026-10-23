/*
 * A008998  a(n) = 2*a(n-1) + a(n-3), with a(0)=1 and a(1)=2.
 * https://oeis.org/A008998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-3), a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Static cache for already computed values up to a certain index. */
    static seq_t cache[10001];      /* enough for n up to 10000 */
    static idx_t cache_len = 0;     /* highest index computed (≥0) */

    /* Initialize base cases on first call. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache_len = 2; /* we have computed indices 0,1,2 */
    }

    /* If the requested index is already computed, return it. */
    if (n <= cache_len)
        return cache[n];

    /* Compute all intermediate values up to n. */
    /* Use idx_t for loop counter; assume n <= 10000 due to cache size. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 0)          val = (seq_t)1;
        else if (i == 1)     val = (seq_t)2;
        else if (i == 2)     val = (seq_t)4;
        else                 val = (seq_t)2 * cache[i - 1] + cache[i - 3];
        cache[i] = val;
    }
    cache_len = n;
    return cache[n];
}

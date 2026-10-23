/*
 * A022360  Fibonacci sequence beginning 0, 26.
 * https://oeis.org/A022360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 0, a(1) = 26
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of the sequence a(n) = 26*A000045(n)  */
seq_t a(idx_t n)
{
    /* static cache to allow arbitrary call order */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements currently cached */

    /* Allocate or extend cache to hold terms up to n */
    if (n >= cache_len) {
        size_t newlen = (size_t)(n + 1);
        cache = (seq_t*)realloc(cache, newlen * sizeof(seq_t));

        /* Compute missing terms */
        for (size_t i = (size_t)cache_len; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)0;
            } else if (i == 1) {
                cache[1] = (seq_t)26;   /* 26*F(1) = 26  */
            } else {
                cache[i] = cache[i - 1] + cache[i - 2];
            }
        }
        cache_len = newlen;
    }

    return cache[n];
}

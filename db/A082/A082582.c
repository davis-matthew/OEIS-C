/*
 * A082582  Expansion of (1 + x^2 - sqrt( 1 - 4*x + 2*x^2 + x^4)) / (2*x) in
 *          powers of x.
 * https://oeis.org/A082582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + Σ_{k=2}^{n-1} a(k)*a(n-1-k) a(0)=a(1)=a(2)=1,
 *            a(3)=2
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 22, u64 n <= 41, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t len  = 0;   /* number of terms currently cached   */
    static size_t cap  = 0;   /* allocated capacity of cache array  */

    /* initialise cache with the first four known terms */
    if (len == 0) {
        cap   = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */

        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)1;   /* a(1) */
        cache[2] = (seq_t)1;   /* a(2) */
        cache[3] = (seq_t)2;   /* a(3) */
        len     = 4;
    }

    /* extend the cache until it contains a(n) */
    while (len <= n) {
        /* grow the array if needed */
        if (len == cap) {
            size_t newcap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;   /* allocation failed */
            cache = tmp;
            cap   = newcap;
        }

        /* compute a(len) using the recurrence */
        seq_t sum = cache[len - 1];            /* a(n) term  */
        for (size_t k = 2; k <= len - 1; ++k) {
            sum += cache[k] * cache[len - 1 - k];   /* a(k)*a(n-k)   */
        }
        cache[len] = sum;
        ++len;
    }

    return cache[(size_t)n];
}

/*
 * A099166  G.f.: (1+x^2)/((1-2x)(1-x-x^2)); a(n)=3a(n-1)-a(n-2)-2a(n-3);
 *          a(n)=5*2^n-L(n+3); a(n)=sum{k=0..n, (L(k)-0^k)2^(n-k)}.
 * https://oeis.org/A099166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3), a(0)=1, a(1)=3, a(2)=9
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;

    /* Static cache for dynamic programming */
    static seq_t *cache = NULL;
    static idx_t cached_last = (idx_t)-1; /* last index computed */
    static idx_t cache_size = 0;          /* allocated size */

    /* Initial allocation */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cached_last = 2;
    }

    /* Return from cache if already computed */
    if (n <= cached_last) return cache[n];

    /* Resize cache if needed */
    if (n + 1 > cache_size) {
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    }

    /* Compute successive values up to n */
    for (idx_t i = cached_last + 1; i <= n; ++i) {
        seq_t t1 = cache[i-1] * (seq_t)3;
        seq_t t2 = cache[i-2];
        seq_t t3 = cache[i-3] * (seq_t)2;
        cache[i] = t1 - t2 - t3;
    }
    cached_last = n;
    return cache[n];
}

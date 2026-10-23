/*
 * A017824  Expansion of 1/(1-x^3-x^4-x^5-x^6-x^7-x^8-x^9-x^10-x^11).
 * https://oeis.org/A017824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=1, a(6)=2, a(7)=3,
 *            a(8)=4, a(9)=6, a(10)=9. For n>10:
 *            a(n)=a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7)+a(n-8)+a(n-9)+a(n-10)+a(n-11)
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 63, u64 n <= 122,
 *            u128 n <= 240, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values, grows as needed */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of elements stored (max index+1) */

    /* initial values for n = 0..10 */
    static const seq_t init[11] = {
        1, 0, 0, 1, 1, 1, 2, 3, 4, 6, 9
    };

    /* lazily allocate and initialise the cache */
    if (!cache) {
        cache_size = 11;  /* indices 0..10 will be present */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure; caller can handle */
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = init[i];
    }

    /* if requested value is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* enlarge cache to accommodate up to index n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;          /* allocation failure */
    cache = new_cache;

    /* compute values from the first missing index up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        /* Sums over the previous 9 positions (indices i-3 .. i-11) */
        for (int k = 3; k <= 11; ++k)
            sum += cache[i - k];
        cache[i] = sum;
    }

    cache_size = n + 1;
    return cache[n];
}

/*
 * A121230  First Hadamard-Sylvester matrix self -similar matrix based on the
 *          Padovan/ Minimal Pisot 3 X 3 matrix as an 9 X 9 matrix:
 *          Characteristic Polynomial:1 - x - x^3 - x^4 - x^5 + 3 x^6 + 2 x^7
 *          - x^9.
 * https://oeis.org/A121230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned, obsc
 * Formula:   a(1)=0, a(2)=13, a(3)=5, a(4)=22, a(5)=42, a(6)=54; a(n) =
 *            a(n-1)+a(n-2)+a(n-3)-a(n-4)+a(n-5)-a(n-6) for n ≥ 7
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for n<1, return 0 safely */
        return (seq_t)0;

    static seq_t *cache      = NULL;   /* values of the sequence */
    static idx_t cache_size  = 0;      /* allocated size (indices 0..cache_size-1) */
    static idx_t last_computed = 0;    /* highest index computed so far */

    /* Ensure the cache is large enough for n */
    if (cache_size < n + 1) {
        idx_t newsize = cache_size ? cache_size * 2 : 8;
        if (newsize < n + 1)
            newsize = n + 1;
        cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        cache_size = newsize;
    }

    /* Set the first six terms on first use */
    if (last_computed < 6) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)22;
        cache[5] = (seq_t)42;
        cache[6] = (seq_t)54;
        /* Keep any previous values for indices >6 unchanged */
        if (last_computed < 6)
            last_computed = 6;
    }

    /* If the requested n has already been computed, return it */
    if (n <= last_computed)
        return cache[n];

    /* Compute new terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* recurrence: a(i-1)+a(i-2)+a(i-3)-a(i-4)+a(i-5)-a(i-6) */
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3]
                 - cache[i-4] + cache[i-5] - cache[i-6];
    }
    last_computed = n;
    return cache[n];
}

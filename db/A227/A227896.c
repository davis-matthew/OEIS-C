/*
 * A227896  32-beat repeating palindromic sequence: digital roots of Fibonacci
 *          numbers indexed by the set of natural numbers not divisible by 2,
 *          3 or 5 (A007775).
 * https://oeis.org/A227896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = a(n-1) - a(n-16) + a(n-17) for n > 17 Initial values:
 *            a(1)=1 a(2)=4 a(3)=8 a(4)=8 a(5)=4 a(6)=5 a(7)=1 a(8)=5 a(9)=4
 *            a(10)=8 a(11)=4 a(12)=5 a(13)=1 a(14)=1 a(15)=5 a(16)=8 a(17)=8
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* initialise cache with first 17 terms on first call */
    if (cache == NULL) {
        /* initial capacity large enough for at least 17 terms */
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */

        static const seq_t init[17] = {
            1,4,8,8,4,5,1,5,4,8,4,5,1,1,5,8,8
        };
        for (idx_t i = 0; i < 17; ++i) cache[i] = init[i];
        cache_len = 17;
    }

    /* return immediately if already computed */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* increase capacity if necessary */
    while (cache_len < n) {
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) { /* allocation failure */
                free(cache);
                cache = NULL;
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute a(k) for k = cache_len + 1 using recurrence */
        idx_t k = cache_len + 1;

        /* indices used in recurrence are 1‑based:
           a(k-1) -> cache[k-2]
           a(k-16) -> cache[k-17]
           a(k-17) -> cache[k-18]      */
        int val = (int)cache[(size_t)(k - 2)]
                 - (int)cache[(size_t)(k - 17)]
                 + (int)cache[(size_t)(k - 18)];

        cache[cache_len++] = (seq_t)val;
    }

    return cache[n - 1];
}

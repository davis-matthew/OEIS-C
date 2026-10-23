/*
 * A182052  Number of independent sets of nodes in C_6 X C_n (n >= 1).
 * https://oeis.org/A182052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 84*a(n-2) + 89*a(n-3) - 575*a(n-4) -
 *            360*a(n-5) + 1301*a(n-6) + 115*a(n-7) - 1032*a(n-8) + 295*a(n-9)
 *            + 119*a(n-10) - 36*a(n-11) - 4*a(n-12) + a(n-13) with a(0)=18,
 *            a(1)=1, a(2)=199, a(3)=1300, a(4)=18995, a(5)=199821,
 *            a(6)=2406862, a(7)=27285777, a(8)=317960739, a(9)=3658040968...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t max_computed = -1;    /* no values yet */

    /* Initial base values (indices 0..12) */
    static const seq_t base_vals[13] = {
        (seq_t)18ULL,
        (seq_t)1ULL,
        (seq_t)199ULL,
        (seq_t)1300ULL,
        (seq_t)18995ULL,
        (seq_t)199821ULL,
        (seq_t)2406862ULL,
        (seq_t)27285777ULL,
        (seq_t)317960739ULL,
        (seq_t)3658040968ULL,
        (seq_t)42338077399ULL,
        (seq_t)488631332773ULL,
        (seq_t)5646974285234ULL
    };

    /* Initialise cache if required */
    if (!initialized) {
        cache_size = 13;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = base_vals[i];
        max_computed = 12;
        initialized = true;
    }

    /* If requested index is already computed, return it */
    if ((idx_t)0 <= n && n <= max_computed)
        return cache[(size_t)n];

    /* Ensure cache is large enough */
    size_t needed = (size_t)n + 1;
    if (needed > cache_size) {
        size_t newcap = cache_size ? cache_size : 13;
        while (newcap < needed)
            newcap <<= 1;
        seq_t *newarr = (seq_t *)malloc(newcap * sizeof(seq_t));
        for (size_t i = 0; i < cache_size; ++i)
            newarr[i] = cache[i];
        free(cache);
        cache = newarr;
        cache_size = newcap;
    }

    /* Compute missing values */
    for (size_t i = (size_t)max_computed + 1; i <= (size_t)n; ++i) {
        seq_t term = 4ULL * cache[i-1] +
                     84ULL * cache[i-2] +
                     89ULL * cache[i-3];
        term -= 575ULL * cache[i-4];
        term -= 360ULL * cache[i-5];
        term += 1301ULL * cache[i-6];
        term += 115ULL * cache[i-7];
        term -= 1032ULL * cache[i-8];
        term += 295ULL * cache[i-9];
        term += 119ULL * cache[i-10];
        term -= 36ULL * cache[i-11];
        term -= 4ULL * cache[i-12];
        term += cache[i-13];
        cache[i] = term;
    }
    max_computed = n;
    return cache[(size_t)n];
}

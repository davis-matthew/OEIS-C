/*
 * A219350  Number of n X 4 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical, diagonal or antidiagonal
 *          neighbors in a random, but sorted with lexicographically
 *          nondecreasing rows and columns, 0..1 n X 4 array.
 * https://oeis.org/A219350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) (n>16)
 *            Initial values: a(1)=4, a(2)=4, a(3)=15, a(4)=35, a(5)=81,
 *            a(6)=174, a(7)=364, a(8)=740, a(9)=1459, a(10)=2778, a(11)=5105,
 *            a(12)=9069, a(13)=15615, a(14)=26129, a(15)=42600, a(16)...
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 63, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1ULL)
        return 0;

    /* Persistent cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* First‑time initialisation */
    if (cache == NULL) {
        cache_cap = 16;
        cache_size = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        static const seq_t init_vals[16] = {
            (seq_t)4ULL,  (seq_t)4ULL,  (seq_t)15ULL, (seq_t)35ULL,
            (seq_t)81ULL, (seq_t)174ULL, (seq_t)364ULL, (seq_t)740ULL,
            (seq_t)1459ULL,(seq_t)2778ULL,(seq_t)5105ULL,
            (seq_t)9069ULL,(seq_t)15615ULL,(seq_t)26129ULL,
            (seq_t)42600ULL,(seq_t)67827ULL
        };
        for (size_t i = 0; i < 16; ++i)
            cache[i] = init_vals[i];
    }

    /* Return earlier term if already known */
    if (n <= cache_size)
        return cache[n - 1];

    size_t target = (size_t)n;
    if (target > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < target)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n using the 9‑term recurrence */
    for (size_t i = cache_size; i < target; ++i) { /* i is zero‑based index for a(i+1) */
        /* i >= 16 ensures all indices i-1 .. i-9 are valid */
        seq_t val = (seq_t)0;
        val += (seq_t)9   * cache[i - 1];
        val -= (seq_t)36  * cache[i - 2];
        val += (seq_t)84  * cache[i - 3];
        val -= (seq_t)126 * cache[i - 4];
        val += (seq_t)126 * cache[i - 5];
        val -= (seq_t)84  * cache[i - 6];
        val += (seq_t)36  * cache[i - 7];
        val -= (seq_t)9   * cache[i - 8];
        val += (seq_t)1   * cache[i - 9];
        cache[i] = val;
    }

    cache_size = target;
    return cache[n - 1];
}

/*
 * A218899  Number of 3Xn arrays of the minimum value of corresponding
 *          elements and their horizontal, diagonal or antidiagonal neighbors
 *          in a random 0..1 3Xn array.
 * https://oeis.org/A218899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:
 *   a(n) = 2*a(n-1) + 7*a(n-2) - 4*a(n-3) - 6*a(n-4) -
 *          10*a(n-5) - 20*a(n-6) + 15*a(n-7) - a(n-8) +
 *          22*a(n-9) + 14*a(n-10) - 5*a(n-11) - 6*a(n-13) - 2*a(n-14)
 *   initial values (n = 1..14):
 *     a(1)=8,  a(2)=16,  a(3)=72,  a(4)=212,  a(5)=786,
 *     a(6)=2594, a(7)=9110, a(8)=31062, a(9)=107510, a(10)=369596,
 *     a(11)=1274760, a(12)=4390640, a(13)=15132044, a(14)=52139518
 */

#include <stddef.h>
#include <stdlib.h>

/* initial values, index 1..14; init_vals[0] unused */
static const seq_t init_vals[15] = {
    0ULL,          /* unused  */
    8ULL,          16ULL, 72ULL,     212ULL,   786ULL,
    2594ULL,      9110ULL, 31062ULL, 107510ULL, 369596ULL,
    1274760ULL,  4390640ULL, 15132044ULL, 52139518ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safe guard for n < 1 */

    /* return known values directly for n <= 14 */
    if (n <= 14) return init_vals[(size_t)n];

    /* memoisation cache */
    static seq_t *cache = NULL;    /* cache[0] = a(1), cache[1] = a(2), ... */
    static size_t  cache_len = 0;   /* number of values stored */
    static size_t  cache_cap = 0;   /* allocated capacity */

    /* initialise cache with the first 14 values on first call */
    if (cache_len == 0) {
        cache_cap = 32;                          /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                    /* OOM: fallback */
        for (size_t i = 1; i <= 14; ++i)
            cache[i - 1] = init_vals[i];
        cache_len = 14;
    }

    /* grow cache if needed */
    if ((idx_t)cache_len < n) {
        size_t needed = (size_t)n;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 16;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;                  /* OOM: fallback */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing values sequentially */
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            /* indices in cache are 0‑based, cache[k-2] = a(k-1), etc. */
            seq_t v = 0;
            v += 2 * cache[(size_t)(k - 2)];
            v += 7 * cache[(size_t)(k - 3)];
            v -= 4 * cache[(size_t)(k - 4)];
            v -= 6 * cache[(size_t)(k - 5)];
            v -=10 * cache[(size_t)(k - 6)];
            v -=20 * cache[(size_t)(k - 7)];
            v +=15 * cache[(size_t)(k - 8)];
            v -= 1 * cache[(size_t)(k - 9)];
            v +=22 * cache[(size_t)(k -10)];
            v +=14 * cache[(size_t)(k -11)];
            v -= 5 * cache[(size_t)(k -12)];
            v -= 6 * cache[(size_t)(k -14)];
            v -= 2 * cache[(size_t)(k -15)];
            cache[(size_t)(k - 1)] = v;
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n - 1];
}

/*
 * A236646  Number of (n+1)X(1+1) 0..2 arrays colored with the maximum plus
 *          the upper median minus the lower median minus the minimum of every
 *          2X2 subblock.
 * https://oeis.org/A236646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+4*a(n-2)+60*a(n-3)+500*a(n-4)+608*a(n-5)+1064*a(n-6)
 *            +512*a(n-7)-14672*a(n-8)-18640*a(n-9)-27776*a(n-10)-53248*a(n-11)
 *            Initial values: a(1)=81, a(2)=484, a(3)=2932, a(4)=17824,
 *            a(5)=108272, a(6)=655100, a(7)=3970468, a(8)=24093844,
 *            a(9)=146112996, a(10)=885471428, a(11)=5368434004
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle out of offset: undefined but return 0 */
    if (n <= 0ULL) return 0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* allocated size */
    static size_t count    = 0;   /* number of terms stored */

    /* Recurrence coefficients */
    static const int coeff[11] = {
        1, 4, 60, 500, 608,
        1064, 512, -14672, -18640, -27776, -53248
    };

    /* Initial 11 terms */
    static const seq_t init[11] = {
        (seq_t)81ULL,
        (seq_t)484ULL,
        (seq_t)2932ULL,
        (seq_t)17824ULL,
        (seq_t)108272ULL,
        (seq_t)655100ULL,
        (seq_t)3970468ULL,
        (seq_t)24093844ULL,
        (seq_t)146112996ULL,
        (seq_t)885471428ULL,
        (seq_t)5368434004ULL
    };

    /* If requested term is within the first 11, return it directly */
    if (n <= 11ULL) return init[n-1];

    /* Ensure cache has enough space for at least n terms */
    if (n > capacity) {
        size_t newcap = capacity ? capacity : 16;
        while (newcap < (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;     /* allocation failure returns 0 */
        cache = tmp;
        capacity = newcap;
    }

    /* Initialise cache with the first 11 terms on first use */
    if (count == 0) {
        for (size_t i = 0; i < 11; ++i) cache[i] = init[i];
        count = 11;
    }

    /* If the required term is already in cache, return it */
    if (n <= count) return cache[n-1];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t cur = count + 1; cur <= n; ++cur) {
        seq_t acc = 0;
        for (int j = 0; j < 11; ++j) {
            seq_t term = cache[cur - 2 - j];   /* cur-1 is 1‑based, cache 0‑based */
            int c = coeff[j];
            if (c >= 0)
                acc += term * (seq_t)c;
            else
                acc -= term * (seq_t)(-c);
        }
        cache[cur-1] = acc;
    }

    count = (size_t)n;
    return cache[n-1];
}

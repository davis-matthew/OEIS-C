/*
 * A165219  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) + 8*a(n-2) + ... + 8*a(n-8) - 36*a(n-9) + c_n
 *            where c_0 = 1, c_n = 2 for 1 ≤ n ≤ 8, c_9 = 1, and c_n = 0 for n
 *            ≥ 10. (This follows from the generating function (t^9 + 2*t^8 +
 *            2*t^7 + 2*t^6 + 2*t^5 + 2*t^4 + 2*t^3 + 2*t^2 + 2*t + 1) /
 *            (36*t^9 - 8*t^8 - 8*t^7 - 8*t^6 - 8*t^5 - 8*t^4...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;   /* number of terms already computed */

    /* initialise on first use */
    if (cache == NULL) {
        cap   = 16;                                /* arbitrary start size */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        len   = 0;
    }

    /* target index as size_t (safe for the expected n ranges) */
    const size_t target = (size_t)n;

    /* compute terms up to n */
    while (len <= target) {
        /* ensure enough capacity for cache[len] */
        if (len >= cap) {
            size_t newcap = (cap << 1) ?: 1;
            seq_t *tmp    = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();          /* memory allocation failed */
            cache = tmp;
            cap   = newcap;
        }

        seq_t sum8   = 0;          /* 8*a(n-1)+...+8*a(n-8) */
        for (int i = 1; i <= 8; ++i) {
            if (len >= (size_t)i)
                sum8 += 8 * cache[len - i];
        }

        seq_t part9 = 0;           /* 36*a(n-9) */
        if (len >= 9)
            part9 = 36 * cache[len - 9];

        seq_t c;                   /* inhomogeneous term */
        if (len == 0)
            c = 1;
        else if (len <= 8)
            c = 2;
        else if (len == 9)
            c = 1;
        else
            c = 0;

        seq_t val = sum8 - part9 + c;
        cache[len] = val;
        ++len;
    }

    return cache[n];
}

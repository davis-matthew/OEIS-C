/*
 * A163968  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A163968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)) - 153*a(n-6)
 *            Initial values: a(0)=1, a(1)=19, a(2)=342, a(3)=6156,
 *            a(4)=110808, a(5)=1994544, a(6)=35901621
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t size = 0;            /* number of terms currently stored */

    /* Initialise the cache on the first call */
    if (!cache) {
        capacity = 16;                 /* initial capacity > 7 */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failed */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)19;
        cache[2] = (seq_t)342;
        cache[3] = (seq_t)6156;
        cache[4] = (seq_t)110808;
        cache[5] = (seq_t)1994544;
        cache[6] = (seq_t)35901621;
        size = 7;                      /* next free index */
    }

    /* If the requested index is already computed, return it */
    if (n < size)
        return cache[n];

    /* Grow the cache if necessary */
    if (capacity <= (size_t)n) {
        size_t new_cap = capacity;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;             /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;    /* allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = size; i <= n; ++i) {
        seq_t sum = cache[i-1];
        sum += cache[i-2];
        sum += cache[i-3];
        sum += cache[i-4];
        sum += cache[i-5];
        seq_t val = sum * (seq_t)17;
        val -= cache[i-6] * (seq_t)153;
        cache[i] = val;
    }
    size = n + 1;                       /* update number of stored terms */

    return cache[n];
}

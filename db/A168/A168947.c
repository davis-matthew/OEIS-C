/*
 * A168947  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n_n + 28*∑_{k=1}^{21}a(n-k) – 406*a(n-22) where n_n = 1
 *            for n=0 or n=22, 2 for 1≤n≤21, and 0 otherwise
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t length   = 0;

    /* number of terms we need to have computed (0 … n inclusive) */
    size_t needed = (size_t)n + 1;

    /* grow the cache if necessary */
    if (needed > capacity) {
        size_t newcap = capacity ? capacity * 2 : 64;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure – abort */
        if (cache) {
            memcpy(tmp, cache, length * sizeof(seq_t));
            free(cache);
        }
        cache    = tmp;
        capacity = newcap;
    }

    /* compute terms up to index n */
    if (needed > length) {
        /* ensure a(0) is set correctly */
        if (length == 0) {
            cache[0] = (seq_t)1;
            length   = 1;
        }

        for (size_t i = length; i < needed; ++i) {
            /* sum_{k=1}^{21} a(i-k) */
            seq_t sum = 0;
            for (size_t k = 1; k <= 21 && k <= i; ++k)
                sum += cache[i - k];

            seq_t val = sum * (seq_t)28;

            /* add n_n */
            if (i == 0 || i == 22)
                val += (seq_t)1;
            else if (i <= 21)
                val += (seq_t)2;

            /* subtract 406*a(i-22) if i>=22 */
            if (i >= 22)
                val -= cache[i - 22] * (seq_t)406;

            cache[i] = val;
        }
        length = needed;
    }

    return cache[(size_t)n];
}

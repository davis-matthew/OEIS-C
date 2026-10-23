/*
 * A171878  a(n) = a(n-1) + a(n-2)*a(n-3) + a(n-4)^a(n-5), with initial terms
 *          0,0,0,0,1.
 * https://oeis.org/A171878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2)*a(n-3) + a(n-4)^a(n-5) with initial
 *            values a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t len = 0;
    static size_t cap = 0;

    /* Initialize cache with first 5 terms */
    if (cache == NULL) {
        cap = 16;                    /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)0;
        cache[4] = (seq_t)1;
        len = 5;
    }

    /* Grow cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort computation */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    if ((size_t)n >= len) {
        for (idx_t i = (idx_t)len; i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];
            seq_t term = a1 + (a2 * a3) + big_pow(a4, a5);
            cache[i] = term;
        }
        len = n + 1;
    }

    return cache[n];
}

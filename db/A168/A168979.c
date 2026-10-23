/*
 * A168979  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*(a(n-1)+…+a(n-22)) - 78*a(n-23) + b_n where b_0 = 1,
 *            b_1…b_22 = 2, b_23 = 1, and b_n = 0 for n>23. This recurrence
 *            follows from the rational generating function of A168979
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i) {
            idx_t idx = (idx_t)i;
            seq_t b;
            if (idx == 0)           b = 1;
            else if (idx <= 22)     b = 2;
            else if (idx == 23)     b = 1;
            else                    b = 0;
            seq_t sum = b;
            size_t maxk = (i < 22) ? i : 22;
            for (size_t k = 1; k <= maxk; ++k) {
                sum += (seq_t)12 * cache[i - k];
            }
            if (i >= 23) {
                sum -= (seq_t)78 * cache[i - 23];
            }
            cache[i] = sum;
        }
        cache_cap = new_cap;
    }
    return cache[(size_t)n];
}

/*
 * A169252  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For 1 ≤ n < 28: a(n) = 45 * (a(0)+a(1)+…+a(n−1)) + 2.
 *            For n = 28: a(28) = 45 * (a(1)+a(2)+…+a(27)) – 1034. For n > 28:
 *            a(n) = 45 * (a(n−27)+a(n−26)+…+a(n−1)) – 1035 * a(n−28)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;

        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t term;
            if (i == 0) {
                term = (seq_t)1;
            } else {
                seq_t sum = 0;
                idx_t start = (i < 28) ? 0 : (i - 27);
                for (idx_t j = start; j < i; ++j) sum += cache[j];
                if (i < 28) {
                    term = (seq_t)45 * sum + (seq_t)2;
                } else if (i == 28) {
                    term = (seq_t)45 * sum - (seq_t)1034;
                } else {
                    term = (seq_t)45 * sum - (seq_t)1035 * cache[i - 28];
                }
            }
            cache[i] = term;
        }
        cache_size = new_size;
    }
    return cache[n];
}

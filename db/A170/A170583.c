/*
 * A170583  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 * a(0) = 1.
 * For 1 ≤ n ≤ 46: a(n) = 2 + 44 * Σ_{k=0}^{n-1} a(k).
 * For n = 47: a(47) = 1 + 44 * Σ_{k=1}^{46} a(k) - 990 * a(0).
 * For n ≥ 48: a(n) = 44 * Σ_{k=1}^{46} a(n-k) - 990 * a(n-47).
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i <= 46) {
                seq_t sum = (seq_t)0;
                for (idx_t j = 0; j < i; ++j)
                    sum += cache[j];
                cache[i] = (seq_t)2 + (seq_t)44 * sum;
            } else if (i == 47) {
                seq_t sum = (seq_t)0;
                for (idx_t j = 1; j <= 46; ++j)
                    sum += cache[j];
                cache[47] = (seq_t)1 + (seq_t)44 * sum - (seq_t)990 * cache[0];
            } else { /* i >= 48 */
                seq_t sum = (seq_t)0;
                for (idx_t j = 1; j <= 46; ++j)
                    sum += cache[i - j];
                cache[i] = (seq_t)44 * sum - (seq_t)990 * cache[i - 47];
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}

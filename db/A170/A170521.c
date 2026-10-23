/*
 * A170521  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the number of reduced words of length n in the Coxeter
   group on 32 generators with relations (S_i)^2 = (S_i S_j)^46 = I:
   a(0) = 1
   a(1) = 32
   a(n) = 31·a(n-1)          for 2 ≤ n ≤ 45
   a(n) = 30·∑_{i=1}^{45} a(n-i) – 465·a(n-46)   for n ≥ 46   */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoized terms, cache[i] = a(i) */
    static size_t cache_len = 0; /* number of terms currently stored */

    size_t target = (size_t)n;
    if (target >= cache_len) {
        size_t new_len = target + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        for (size_t i = cache_len; i <= target; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)32;
            } else if (i < 46) {
                cache[i] = cache[i-1] * (seq_t)31;
            } else { /* i >= 46 */
                seq_t sum45 = 0;
                for (size_t j = i-45; j < i; ++j) {
                    sum45 += cache[j];
                }
                cache[i] = (seq_t)30 * sum45 - (seq_t)465 * cache[i-46];
            }
        }
        cache_len = new_len;
    }
    return cache[target];
}

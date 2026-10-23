/*
 * A170042  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170042
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

#include <stdlib.h>

/* Recurrence:
   a(0) = 1;
   for 1 <= n < 36:   a(n) = 2 + 31 * Σ_{i=1}^{min(35,n)} a(n-i)
   for n >= 36:       a(n) = 31 * Σ_{i=1}^{35} a(n-i) - 496 * a(n-36)
*/
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t size = 0;

    if (!cache) {
        cap = 64;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    if (n >= size) {
        if ((size_t)n >= cap) {
            size_t new_cap = cap;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        for (idx_t i = size; i <= n; ++i) {
            seq_t sum = 0;
            idx_t limit = (i < 36 ? i : 35);
            for (idx_t k = 1; k <= limit; ++k) sum += cache[i - k];

            if (i < 36) {
                seq_t bi = (i == 0 ? (seq_t)1 : (seq_t)2);
                cache[i] = bi + (seq_t)31 * sum;
            } else {
                cache[i] = (seq_t)31 * sum - (seq_t)496 * cache[i - 36];
            }
        }
        size = n + 1;
    }
    return cache[n];
}

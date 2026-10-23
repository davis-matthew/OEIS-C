/*
 * A170282  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   sequence satisfies the recurrence * a(0) = 1; * a(n) =
 *            31*(a(n-1)+…+a(n-40)) + 2 for 1 ≤ n ≤ 40; * a(41) =
 *            31*(a(40)+…+a(1)) – 496*a(0) + 1; * a(n) = 31*(a(n-1)+…+a(n-40))
 *            – 496*a(n-41) for n ≥ 42
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of entries currently stored */

    if (n + 1 > cache_size) {
        size_t old_size = cache_size;
        cache_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));

        for (idx_t i = old_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = 1;
            } else if (i <= 40) {                  /* 1 ≤ i ≤ 40 */
                seq_t sum = 0;
                for (idx_t j = 0; j < i; ++j) {
                    sum += cache[j];
                }
                cache[i] = ((seq_t)31 * sum) + 2;
            } else if (i == 41) {                  /* i == 41 */
                seq_t sum = 0;
                for (idx_t j = 1; j <= 40; ++j) {
                    sum += cache[j];
                }
                cache[i] = ((seq_t)31 * sum)
                           - ((seq_t)496 * cache[0])
                           + 1;
            } else {                               /* i ≥ 42 */
                seq_t sum = 0;
                for (idx_t k = i - 1; k > i - 41; --k) {
                    sum += cache[k];
                }
                cache[i] = ((seq_t)31 * sum)
                           - ((seq_t)496 * cache[i - 41]);
            }
        }
    }

    return cache[n];
}

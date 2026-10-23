/*
 * A151792  Partial sums of A151791.
 * https://oeis.org/A151792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = partial sums of t(i) where t(1)=1 and for i>1,
 *            t(i)=7*6^(popcount(i-1)-1)
 * Range:     u8 n <= 7, u16 n <= 61, u32 n <= 3182, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (n == 0) return 0;

    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure, unlikely for test harness */
        cache = tmp;
        for (size_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 0;
            } else if (i == 1) {
                cache[1] = cache[0] + (seq_t)1;
            } else {
                uint8_t k = __builtin_popcountll((unsigned long long)(i - 1));
                seq_t pow = 1;
                for (uint8_t j = 1; j < k; ++j) pow *= (seq_t)6;
                seq_t term = (seq_t)7 * pow;
                cache[i] = cache[i - 1] + term;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}

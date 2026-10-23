/*
 * A170060  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 2*a(n-1) - a(n-2) +
 *            2*a(n-3) - a(n-4) + … + 2*a(n-35) - a(n-36) for n > 36, with
 *            initial values obtained from (1 + t + … + t^36) / (1 - 2*t + t^2
 *            - 2*t^3 + … + t^36) (a(0)=1, a(1)=3, a(2)=6, …)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n < cache_len)
        return cache[n];

    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        abort();   /* memory allocation failed */

    cache = tmp;

    for (size_t i = cache_len; i < new_len; ++i) {
        seq_t sum = 0;
        idx_t limit = (i <= 36) ? (idx_t)i : 36;

        for (idx_t k = 1; k <= limit; ++k) {
            seq_t term = cache[i - k];
            if (k & 1)        /* coefficient –2 */
                sum += -2 * term;
            else                /* coefficient +1 */
                sum += term;
        }

        seq_t rhs = (i <= 36) ? 1 : 0;
        cache[i] = rhs - sum;
    }

    cache_len = new_len;
    return cache[n];
}

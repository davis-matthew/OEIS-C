/*
 * A144367  Shifts 3 places left under Dirichlet convolution.
 * https://oeis.org/A144367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  eigen, nonn
 * Formula:   a(n) = 1 for 1 <= n <= 3; for n > 3, /* a(n) = Σ_{d | (n-3)}
 *            a(d) * a((n-3)/d) (Dirichlet convolution shifted by 3)
 * Range:     u8 n <= 23, u16 n <= 47, u32 n <= 95, u64 n <= 191,
 *            u128 n <= 383, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t max_computed = 0;

    if (n == 0) return 0;   /* safety: OEIS offset starts at 1 */

    /* allocate or grow the cache array as needed */
    if (capacity <= n) {
        idx_t new_cap = capacity ? capacity << 1 : 4;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = new_cap;
    }

    /* compute any missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 3) {
            cache[i] = (seq_t)1;
        } else {
            idx_t m = i - 3;
            seq_t sum = (seq_t)0;
            for (idx_t d = 1; d * d <= m; ++d) {
                if (m % d == 0) {
                    idx_t e = m / d;
                    seq_t prod = cache[d] * cache[e];
                    sum += prod;
                    if (d != e) sum += prod;   /* count the reverse pair separately */
                }
            }
            cache[i] = sum;
        }
    }
    max_computed = n;
    return cache[n];
}

/*
 * A212846  Polylogarithm li(-n,-1/2) multiplied by (3^(n+1))/2.
 * https://oeis.org/A212846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 a(n+1) = -3*a(n) + 2*Σ_{k=0..n} C(n,k)*a(k)*a(n-k) (n ≥
 *            0)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 21, i128 n <= 35,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised values */
    static size_t capacity = 0;   /* current allocated size */

    /* initial allocation and first term */
    if (capacity == 0) {
        capacity = 1;
        cache = seq_realloc(NULL, 0, capacity);
        cache[0] = (seq_t)1;
    }

    /* extend cache if needed */
    size_t required = (size_t)n + 1;            /* we need index n */
    if (required > capacity) {
        size_t old_cap = capacity;
        cache = seq_realloc(cache, old_cap, required);
        capacity = required;

        /* compute new entries */
        for (size_t i = old_cap; i < capacity; ++i) {
            if (i == 0) {            /* already set to 1 in initial allocation */
                cache[0] = (seq_t)1;
                continue;
            }

            size_t m = i - 1;            /* n in the recurrence for a(i) */
            seq_t sum = 0;
            seq_t bin = 1;               /* binom(m,0) */

            for (size_t k = 0; k <= m; ++k) {
                seq_t prod = bin * cache[k] * cache[m - k];
                sum += prod;

                if (k < m) {             /* binom(m, k+1) = binom(m,k) * (m-k)/(k+1) */
                    bin = bin * (seq_t)(m - k) / (seq_t)(k + 1);
                }
            }

            cache[i] = -3 * cache[i - 1] + 2 * sum;
        }
    }

    return cache[n];
}

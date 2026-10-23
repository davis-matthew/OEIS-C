/*
 * A167957  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^16 = I.
 * https://oeis.org/A167957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 15: a(n) = 39*∑_{j=1}^{n} a(n‑j) + 2 For n
 *            = 16: a(16) = 39*∑_{j=1}^{15} a(16‑j) + 1 – 780*a(0) For n ≥ 17:
 *            a(n) = 39*∑_{j=1}^{15} a(n‑j) – 780*a(n‑16)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t cache[20001];
static size_t  last_index = 0;
static bool    initialized = false;

seq_t a(idx_t n)
{
    /* initialise the cache on first call */
    if (!initialized) {
        cache[0] = 1;
        last_index = 0;
        initialized = true;
    }

    /* return the known first element immediately */
    if (n == 0) return cache[0];

    /* compute missing values up to n */
    if (last_index < (size_t)n) {
        for (idx_t i = last_index + 1; i <= n; ++i) {
            if (i <= 15) {                       /* n = 1 … 15   */
                seq_t sum = 0;
                for (idx_t j = 1; j <= i; ++j)
                    sum += cache[i - j];
                cache[i] = 39 * sum + 2;
            } else if (i == 16) {                 /* n = 16      */
                seq_t sum = 0;
                for (idx_t j = 1; j <= 15; ++j)
                    sum += cache[i - j];
                cache[i] = 39 * sum + 1 - 780 * cache[0];
            } else {                               /* n ≥ 17      */
                seq_t sum = 0;
                for (idx_t j = 1; j <= 15; ++j)
                    sum += cache[i - j];
                cache[i] = 39 * sum - 780 * cache[i - 16];
            }
        }
        last_index = (size_t)n;
    }
    return cache[n];
}

/*
 * A169512  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for 1 ≤ n ≤ 33: a(n) = 17 * Σ_{k=0}^{n-1} a(k) + 2;
 *            for n ≥ 34: a(n) = 17 * Σ_{k=1}^{33} a(n‑k) – 153 * a(n‑34)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoised values */
    static size_t cache_len = 0;     /* number of terms currently stored  */

    /* if we already have the requested term, return it immediately */
    if ((idx_t)cache_len > n) {
        return cache[n];
    }

    /* extend the cache so that it contains indices up to n */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        abort();                      /* allocation failure */
    }
    cache = tmp;
    size_t old_len = cache_len;
    cache_len = new_len;

    /* compute the missing terms */
    for (idx_t i = (idx_t)old_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i <= 33) {
            /* a(i) = 17 * sum_{k=0}^{i-1} a(k) + 2 */
            seq_t sum = 0;
            for (idx_t j = 0; j < i; ++j) {
                sum += cache[j];
            }
            cache[i] = (seq_t)17 * sum + (seq_t)2;
        } else {
            /* a(i) = 17 * sum_{k=1}^{33} a(i-k) – 153 * a(i-34) */
            seq_t sum33 = 0;
            for (idx_t j = i - 33; j <= i - 1; ++j) {
                sum33 += cache[j];
            }
            cache[i] = (seq_t)17 * sum33 - (seq_t)153 * cache[i - 34];
        }
    }

    return cache[n];
}

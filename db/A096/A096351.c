/*
 * A096351  Number of meaningfully different ways to design a neutral
 *          single-elimination tournament with n teams.
 * https://oeis.org/A096351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(n) = ((n! / ((n/2)!*((n/2)!))) * (a(n/2))^2) / 2 if n
 *            even a(n) = ((n! / (((n-1)/2)!*((n+1)/2)!)) * a((n+1)/2) *
 *            a((n-1)/2)) if n odd > 1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 23, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* helper: compute binomial coefficient C(n,k) for 0 <= k <= n */
static seq_t binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n - k + i));
        res = res / ((seq_t)i);   /* exact division */
    }
    return res;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;       /* offset does not matter for n < 1 */

    static seq_t *cache = NULL;
    static idx_t cached_n = 0;

    if (n > cached_n) {
        idx_t new_size = n + 1;                   /* include index 0 unused */
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = 1;
                continue;
            }
            if ((i & 1) == 0) { /* even */
                idx_t k = i >> 1;
                seq_t b = binom(i, k);
                seq_t temp = cache[k];
                temp = temp * temp;
                temp = temp * b;
                temp = temp / 2;
                cache[i] = temp;
            } else { /* odd > 1 */
                idx_t k1 = (i + 1) >> 1;
                idx_t k2 = (i - 1) >> 1;
                seq_t b = binom(i, k1);             /* same as C(i,k2) */
                seq_t temp = cache[k1] * cache[k2];
                temp = temp * b;
                cache[i] = temp;
            }
        }
        cached_n = n;
    }
    return cache[n];
}

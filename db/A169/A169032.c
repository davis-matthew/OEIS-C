/*
 * A169032  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 19, a(n) = 18 * a(n-1) for 2 ≤ n ≤ 23, a(n) =
 *            17 * Σ_{k=1}^{23} a(n-k) + 12 * a(n-24) (n ≥ 24) For n = 24 an
 *            additional +1 is added because the numerator has a coefficient 1
 *            at t^24
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;

    /* lazy initialization: cache[0] = 1 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        max_cached = 0;
    }

    /* already computed? */
    if (n <= max_cached)
        return cache[n];

    /* allocate up to n */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);

    /* remember the old maximum before replacement */
    idx_t old_max = max_cached;
    cache = new_cache;
    max_cached = n;

    /* fill missing values */
    for (idx_t i = old_max + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)19;
        } else if (i <= 23) {
            cache[i] = cache[i - 1] * (seq_t)18;
        } else {  /* i >= 24 */
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= 23; ++j)
                sum += cache[i - j];
            sum *= (seq_t)17;                          /* 17 * Σ a(n-k) */
            sum += cache[i - 24] * (seq_t)12;          /* +12 * a(n-24) */
            if (i == 24)
                sum += (seq_t)1;                       /* +1 for n == 24 */
            cache[i] = sum;
        }
    }

    return cache[n];
}

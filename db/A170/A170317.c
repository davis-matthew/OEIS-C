/*
 * A170317  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * a(n) = 20 · 19^(n‑1) for 1 ≤ n ≤ 41 * a(n) =
 *            18·a(n‑1)+18·a(n‑2)+…+18·a(n‑41) – 171·a(n‑42) for n ≥ 42 * This
 *            implements OEIS A170317 – number of reduced words of length n *
 *            in a Coxeter group on 20 generators with relations (S_i)^2 =
 *            (S_i S_j)^42 = I
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoisation cache – grows on demand */
    static seq_t *cache = NULL;   /* array of computed a(i) */
    static idx_t cache_len = 0;   /* number of valid entries in cache */
    static idx_t cache_cap = 0;   /* allocated capacity of cache */

    /* Pre‑compute powers of 19 needed for the first 41 terms */
    static seq_t pow19[42];
    static int pow19_done = 0;
    if (!pow19_done) {
        pow19[0] = (seq_t)1;
        for (idx_t i = 1; i < 42; ++i)
            pow19[i] = pow19[i-1] * (seq_t)19;
        pow19_done = 1;
    }

    /* Ensure the cache can hold entries up to n */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* out of memory – caller will ignore */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Compute all missing values up to n inclusively */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i < 42) {                     /* 1 ≤ i ≤ 41  */
            val = (seq_t)20 * pow19[i-1];
        } else {                                 /* recurrence for i ≥ 42 */
            seq_t sum = (seq_t)0;
            for (int k = 1; k <= 41; ++k)
                sum += (seq_t)18 * cache[i - k];
            sum -= (seq_t)171 * cache[i - 42];
            val = sum;
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}

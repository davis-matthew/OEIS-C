/*
 * A164627  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let a(0)..a(6) = 1, 16, 240, 3600, 54000, 810000, 12150000. For
 *            n = 7: a(7) = 14*(a(6)+...+a(1)) - 105*a(0) + 1 For n ≥ 8: a(n)
 *            = 14*(a(n-1)+...+a(n-6)) - 105*a(n-7) (linear recurrence)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[7] = {
    (seq_t)1,
    (seq_t)16,
    (seq_t)240,
    (seq_t)3600,
    (seq_t)54000,
    (seq_t)810000,
    (seq_t)12150000
};

seq_t a(idx_t n) {
    /* Naïve memoization with static array */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* one‑past‑last allocated index */
    static idx_t last_computed = 0;   /* highest index that has been computed */

    if (n < 7) {                      /* initial values are known */
        return init_vals[n];
    }

    /* Ensure cache has enough storage */
    if (cache_size == 0) {
        cache_size = (n >= 7 ? n + 1 : 7);
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */
        for (idx_t i = 0; i < 7; ++i) cache[i] = init_vals[i];
        last_computed = 6;
    } else if (cache_size <= n) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failed */
        cache = tmp;
        /* initialise newly allocated part to zero */
        for (idx_t i = cache_size; i <= n; ++i) cache[i] = 0;
        cache_size = n + 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* 14 * (a[i-1] + ... + a[i-6]) */
        for (idx_t k = 1; k <= 6; ++k) {
            sum += (seq_t)14 * cache[i - k];
        }
        /* subtract 105 * a[i-7] */
        sum -= (seq_t)105 * cache[i - 7];
        /* special case for n == 7: add the numerator constant 1 */
        if (i == 7) {
            sum += (seq_t)1;
        }
        cache[i] = sum;
    }
    last_computed = n;

    return cache[n];
}

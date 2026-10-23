/*
 * A165269  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence a(n) satisfies the rational generating function G(t) =
 *            (t^9 + 2*t^8 + ... + 2*t + 1)/(66*t^9 - 11*t^8 - ... - 11*t +
 *            1). The coefficients satisfy the recurrence a_n = 11 *
 *            Σ_{i=1}^{8} a_{n-i} - 66 * a_{n-9} + b_n, where b_n = 1 for n =
 *            0 or 9, b_n = 2 for 1 ≤ n ≤ 8, and b_n = 0 otherwise
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n) {
    /* Persistent cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* allocated slots (≥ computed_max) */
    static idx_t computed_max = 0;  /* number of terms actually computed (top index + 1) */

    /* Grow cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = cache_size ? cache_size * 2 : 64;
        while (new_size <= n) new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure → zero */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute terms from the first missing up to n */
    for (idx_t i = computed_max; i <= n; ++i) {
        /* b_i */
        seq_t b = 0;
        if (i == 0 || i == 9)       b = 1;
        else if (i <= 8 && i >= 1)  b = 2;

        /* Σ_{j=1}^{8} a_{i-j}  (only terms with j ≤ i) */
        seq_t sum = 0;
        for (idx_t j = 1; j <= 8 && i >= j; ++j)
            sum += cache[i - j];

        seq_t term1 = 11 * sum;
        seq_t term2 = (i >= 9) ? (66 * cache[i - 9]) : 0;

        seq_t value = term1 - term2 + b;
        cache[i] = value;
    }

    computed_max = n + 1;  /* 0 … n are now computed */
    return cache[n];
}

/*
 * A128023  a(n) = A128022(n)/n.
 * https://oeis.org/A128023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1, a(n) = ceil((sum_{k=1}^{n-1} k * a(k)) / n)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for previously computed values */
    static seq_t *arr = NULL;      /* array of a(1), a(2), ... */
    static size_t cap = 0;         /* current capacity of arr */
    static size_t max_cached = 0;  /* largest n already computed */
    static seq_t total_S = 0;      /* S(max_cached) = Σ_{k=1}^{max_cached} k*a(k) */

    /* lazy initialization: compute a(1) */
    if (max_cached == 0) {
        arr = (seq_t*)malloc(sizeof(seq_t) * 1);
        arr[0] = (seq_t)1;        /* a(1) = 1 */
        total_S = (seq_t)1;       /* S(1) = 1*1 */
        cap = 1;
        max_cached = 1;
    }

    if (n <= max_cached) {
        return arr[(size_t)(n - 1)];
    }

    /* Ensure capacity for new terms */
    if (n > cap) {
        seq_t *new_arr = (seq_t*)realloc(arr, sizeof(seq_t) * (size_t)n);
        if (!new_arr) return (seq_t)0; /* allocation failure, return 0 */
        arr = new_arr;
        cap = (size_t)n;
    }

    /* Compute terms from max_cached+1 up to n */
    for (idx_t i = (idx_t)(max_cached + 1); i <= n; ++i) {
        seq_t denom = (seq_t)i;
        seq_t a_i = (total_S + denom - (seq_t)1) / denom;  /* ceil division */
        arr[(size_t)(i - 1)] = a_i;

        /* Update cumulative sum S(i) = S(i-1) + i*a_i */
        seq_t i_mul_ai = (seq_t)i * a_i;
        total_S = total_S + i_mul_ai;
    }

    max_cached = n;
    return arr[(size_t)(n - 1)];
}

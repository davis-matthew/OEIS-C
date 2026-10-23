/*
 * A169550  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1. For 1 ≤ n ≤ 34, a(n) = 7 * Σ_{k=1}^{n} a(n-k) + 2. For n
 *            = 35, a(35) = 1 + 7 * Σ_{k=1}^{34} a(35-k) - 28 * a(0). For n ≥
 *            36, a(n) = 7 * Σ_{k=1}^{34} a(n-k) - 28 * a(n-35)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    /* allocate array for up to n terms */
    size_t len = (size_t)n + 1;
    seq_t *arr = (seq_t*)malloc(len * sizeof(seq_t));
    if (!arr) return (seq_t)0;   /* out‑of‑memory, return 0 as fallback */

    arr[0] = (seq_t)1;

    /* compute terms up to 34 */
    for (idx_t i = 1; i <= n && i <= 34; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 0; j < i; ++j)
            sum += arr[j];
        arr[i] = (seq_t)7 * sum + (seq_t)2;
    }
    if (n <= 34) {
        seq_t res = arr[n];
        free(arr);
        return res;
    }

    /* compute a(35) */
    seq_t sum34 = (seq_t)0;
    for (idx_t k = 1; k <= 34; ++k)
        sum34 += arr[35 - k];          /* a[34] down to a[1] */
    arr[35] = (seq_t)1 + (seq_t)7 * sum34 - (seq_t)28 * arr[0];

    if (n == 35) {
        seq_t res = arr[35];
        free(arr);
        return res;
    }

    /* compute subsequent terms using the recurrence of order 35 */
    for (idx_t i = 36; i <= n; ++i) {
        seq_t sumLast34 = (seq_t)0;
        for (idx_t k = 1; k <= 34; ++k)
            sumLast34 += arr[i - k];
        arr[i] = (seq_t)7 * sumLast34 - (seq_t)28 * arr[i - 35];
    }

    seq_t res = arr[n];
    free(arr);
    return res;
}

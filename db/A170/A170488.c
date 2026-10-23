/*
 * A170488  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence derived from the generating function * G(t) = (t^45 +
 *            2*t^44 + … + 2*t + 1) / (1035*t^45 – 45*t^44 – … – 45*t + 1). *
 *            For n >= 46, * a(n) = 45 * (a(n-1)+ … + a(n-44)) – 1035 *
 *            a(n-45) * For 0 <= n <= 45, * a(n) = N_n + 45 * (a(n-1)+ … +
 *            a(0)) if n < 45 * a(45) = 1 + 45 * (a(44)+ … + a(...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

extern seq_t a(idx_t n);

/* Compute the nth term of A170488 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* Allocate an array to hold all terms up to n */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr) return (seq_t)0;            /* allocation failed → return 0 */

    arr[0] = (seq_t)1;                    /* a(0) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t N = (i == 45) ? (seq_t)1 : (seq_t)2;  /* N_i */
        seq_t sum = (seq_t)0;
        idx_t kmax;

        if (i <= 44) {                    /* 1 <= i <= 44 */
            kmax = i;
            for (idx_t k = 1; k <= kmax; ++k)
                sum += arr[i - k];
            arr[i] = N + (seq_t)45 * sum;
        } else if (i == 45) {             /* i == 45 */
            kmax = 44;
            for (idx_t k = 1; k <= kmax; ++k)
                sum += arr[i - k];
            arr[i] = N + (seq_t)45 * sum - (seq_t)1035 * arr[0];
        } else {                          /* i >= 46 */
            kmax = 44;
            for (idx_t k = 1; k <= kmax; ++k)
                sum += arr[i - k];
            arr[i] = (seq_t)45 * sum - (seq_t)1035 * arr[i - 45];
        }
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}

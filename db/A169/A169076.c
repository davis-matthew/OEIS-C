/*
 * A169076  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence for A169076:
   a(0) = 1.
   For n ≥ 1 let c(n) = 2 for 1 ≤ n ≤ 24, c(25) = 1, otherwise 0.
   Then
        a(n) = c(n) + 13 · Σ_{k=1}^{min(24,n)} a(n‑k) – 91 · a(n‑25)
   (for n < 25 the last term is zero).
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    size_t sz = (size_t)(n + 1);

#ifdef __cplusplus
    seq_t *arr = new seq_t[sz];
#else
    seq_t *arr = (seq_t *)malloc(sz * sizeof(seq_t));
#endif
    if (!arr) abort();

    arr[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t kmax = (i < 24 ? i : 24);
        for (idx_t k = 1; k <= kmax; ++k) sum += arr[i - k];

        seq_t coeff = 0;
        if (i <= 24)       coeff = (seq_t)2;
        else if (i == 25) coeff = (seq_t)1;   /* coeff is 0 otherwise */

        seq_t term = sum * (seq_t)13 + coeff;
        if (i >= 25) term -= (seq_t)91 * arr[i - 25];

        arr[i] = term;
    }

    seq_t result = arr[n];

#ifdef __cplusplus
    delete[] arr;
#else
    free(arr);
#endif

    return result;
}

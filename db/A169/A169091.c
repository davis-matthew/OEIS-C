/*
 * A169091  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28*(a(n-1)+a(n-2)+…+a(n-24)) – 406*a(n-25) for n ≥ 25,
 *            with initial values obtained from the power‑series expansion of
 *            the generating function: a(0)=1, a(1)=30, …, a(24)=… The first
 *            25 terms are computed by the same recurrence with a constant
 *            term: a(n)=28*sum_{i=1}^{min(24,n)}a(n-i)+2 (fo...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array to hold terms up to n */
    size_t N = (size_t)n + 1;
    seq_t *arr = (seq_t*)malloc(N * sizeof(seq_t));
    if (!arr) return 0;   /* fallback on allocation failure */

    /* initial term */
    arr[0] = 1;

    /* compute terms from 1 to n */
    for (size_t i = 1; i < N; ++i) {
        seq_t sum = 0;
        for (size_t j = 1; j <= 24 && i >= j; ++j) {
            sum += arr[i - j];
        }

        seq_t next;
        if (i <= 24) {
            /* 1 ≤ i ≤ 24 : add constant 2 */
            next = sum * (seq_t)28 + (seq_t)2;
        } else if (i == 25) {
            /* i = 25 : special constant 1 and subtract 406*a(0) */
            next = sum * (seq_t)28 - (seq_t)406 * arr[0] + (seq_t)1;
        } else {
            /* i ≥ 26 : homogeneous recurrence */
            next = sum * (seq_t)28 - (seq_t)406 * arr[i - 25];
        }

        arr[i] = next;
    }

    seq_t res = arr[n];
    free(arr);
    return res;
}

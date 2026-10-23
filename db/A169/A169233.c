/*
 * A169233  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A169233: Number of reduced words of length n in Coxeter group on 28 generators
 *  G.f.: (t^28 + 2 t^27 + … + 2 t + 1)/(1 - 26 t - 26 t^2 - … - 26 t^27 + 351 t^28)
 *
 *  Recurrence:
 *    a(0) = 1
 *    For 1 ≤ n ≤ 27:    a(n) = 2 + 26 * Σ_{k=0}^{n-1} a(k)
 *    a(28)          = 1 + 26 * Σ_{k=0}^{27} a(k)
 *    For n ≥ 29:       a(n) = 26 * Σ_{k=n-27}^{n-1} a(k) - 351 * a(n-28)
 *
 *  The implementation uses a simple dynamic array and computes the sequence
 *  up to the requested index n. All arithmetic is performed using seq_t,
 *  which supports arbitrary‑precision if needed.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate array of length n+1 */
    size_t len = (size_t)(n + 1);
    seq_t *arr = (seq_t*)malloc(len * sizeof(seq_t));
    if (arr == NULL) return 0;   /* allocation failure – unlikely for n ≤ 10000 */

    /* compute a[0..28] using the inhomogeneous part */
    arr[0] = (seq_t)1;
    seq_t sum = arr[0];          /* cumulative sum of a[0..i] */
    for (idx_t i = 1; i <= 28 && i <= n; ++i) {
        seq_t term;
        if (i == 28)
            term = (seq_t)1 + (seq_t)26 * sum;          /* P[28] = 1 */
        else
            term = (seq_t)2 + (seq_t)26 * sum;          /* P[1..27] = 2 */
        arr[i] = term;
        sum += arr[i];
    }

    if (n <= 28) {
        seq_t result = arr[n];
        free(arr);
        return result;
    }

    /* compute a[29..n] using the homogeneous recurrence */
    for (idx_t i = 29; i <= n; ++i) {
        seq_t s = 0;
        /* sum of the last 27 terms a[i-27] .. a[i-1] */
        for (idx_t j = 1; j <= 27; ++j)
            s += arr[i - j];
        arr[i] = (seq_t)26 * s - (seq_t)351 * arr[i - 28];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}

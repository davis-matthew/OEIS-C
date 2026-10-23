/*
 * A170292  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A170292.
    a(0) = 1
    for 1 ≤ n < 82:     a(n) = 43*42^(n-1)
    for n ≥ 82:         a(n) = 21*a(n-41) + Σ_{k=n-40}^{n-1} a(k)   (sum of the previous 40 terms) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for all terms up to n */
    size_t N = (size_t)n + 1;
    seq_t *arr = (seq_t*)malloc(N * sizeof(seq_t));
    if (!arr) return 0;   /* allocation failed; caller will not get a correct value */

    /* Compute terms 0 .. 81 */
    arr[0] = (seq_t)1;
    seq_t pow42 = (seq_t)1;              /* 42^(n-1), starts with 42^0 for n=1 */
    for (size_t i = 1; i < N && i < 82; ++i) {
        arr[i] = (seq_t)43 * pow42;      /* a(i) = 43*42^(i-1) */
        pow42 = pow42 * (seq_t)42;       /* update for next exponent */
    }

    /* If n < 82 we are done */
    if (n < 82) {
        seq_t result = arr[(size_t)n];
        free(arr);
        return result;
    }

    /* For n ≥ 82: use the recurrence.
       Initialise sum of the last 40 terms: a[42]..a[81] */
    seq_t sum_last40 = 0;
    for (size_t j = 42; j <= 81; ++j) {
        sum_last40 += arr[j];
    }

    /* Compute remaining terms */
    for (size_t i = 82; i <= n; ++i) {
        seq_t term = (seq_t)21 * arr[i - 41] + sum_last40;
        arr[i] = term;
        sum_last40 += term;                 /* add new term to the running sum */
        sum_last40 -= arr[i - 40];          /* remove the term that slides out of the window */
    }

    seq_t result = arr[(size_t)n];
    free(arr);
    return result;
}

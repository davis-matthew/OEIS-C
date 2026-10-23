/*
 * A169408  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   the sequence satisfies the linear recurrence derived from its
 *            generating function G(t) = (1 + 2∑_{k=1}^{31} t^k + t^32) / (1 -
 *            9∑_{k=1}^{31} t^k + 45 t^32). For n ≥ 32 the coefficients follow
 *            a(n) = 9 · (a(n-1)+…+a(n-31)) – 45 · a(n-32), with the special
 *            inhomogeneous term for n = 32: a(32) = 9 ·...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: all numeric operations use seq_t which may be a builtin
   unsigned integer type (8‑128 bits) or an arbitrary‑precision class.
   The code works for both cases. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate array for all needed values */
    seq_t *a_arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!a_arr) return (seq_t)0;  /* allocation failure */
    a_arr[0] = (seq_t)1;

    /* Compute terms 1 … 31 directly via the explicit formula */
    idx_t upto = n < 31 ? (idx_t)n : (idx_t)31;
    seq_t pow10 = (seq_t)1;          /* 10^0 initially */
    for (idx_t i = 1; i <= upto; ++i) {
        a_arr[i] = pow10 * (seq_t)11;
        pow10 = pow10 * (seq_t)10;  /* update for next exponent */
    }

    if (n <= 31) {
        seq_t result = a_arr[(idx_t)n];
        free(a_arr);
        return result;
    }

    /* Prepare the sliding sum of the last 31 values.
       For n = 32 we need the sum of a_arr[1] … a_arr[31] */
    seq_t sum31 = (seq_t)0;
    for (idx_t i = 1; i <= 31; ++i) {
        sum31 += a_arr[i];
    }

    /* Compute a(32) using the special inhomogeneous term */
    a_arr[32] = sum31 * (seq_t)9 - (seq_t)44;

    /* For n ≥ 33 use the recurrence with sliding window */
    for (idx_t i = 33; i <= n; ++i) {
        /* Update sliding sum: add new term, remove the oldest term */
        sum31 += a_arr[i - 1];
        sum31 -= a_arr[i - 32];

        /* a(i) = 9 * sum31 – 45 * a(i-32) */
        a_arr[i] = sum31 * (seq_t)9 - a_arr[i - 32] * (seq_t)45;
    }

    seq_t result = a_arr[n];
    free(a_arr);
    return result;
}

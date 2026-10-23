/*
 * A170200  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170200
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

/* Sequence A170200 – number of reduced words of length n in a Coxeter
 * group with 47 generators and relations (S_i)^2 = (S_i S_j)^39 = I.
 *
 * The generating function is:
 *   P(t) = 1 + 2t + 2t^2 + … + 2t^38 + t^39
 *   Q(t) = 1 - 45t - 45t^2 - … - 45t^38 + 1035t^39
 *
 * Let a_n denote the desired sequence.
 * Then Q(t) * A(t) = P(t) which yields the recurrence:
 *
 *   a_0 = 1
 *   For 1 ≤ n ≤ 38:     a_n = 2 + 45 * (a_0 + a_1 + … + a_{n-1})
 *   For n ≥ 39:          a_n = P_n + 45 * (a_{n-1} + … + a_{n-38})
 *                        - 1035 * a_{n-39}
 *   where P_0 = 1, P_n = 2 for 1 ≤ n ≤ 38, P_39 = 1 and P_n = 0 otherwise.
 *
 * This implementation computes the terms iteratively up to n.
 * It uses only integer arithmetic on the type seq_t, which is
 * guaranteed to support the required operations.
 */

#include <stdlib.h>   /* for malloc, free */

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    /* Allocate array for coefficients a[0..n] */
    seq_t *coeff = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (coeff == NULL) { return (seq_t)0; }  /* allocation failure */

    /* ------------------------------------------------------------------
     * Compute terms 0 … 38 using the first part of the recurrence.
     * ------------------------------------------------------------------ */
    seq_t sum = 0;   /* sum of a[0] … a[i-1] for current i */
    if (n >= 0) {
        coeff[0] = 1;         /* a_0 = 1 */
        sum = coeff[0];
    }

    idx_t i, lim = (n < 38 ? n : 38);
    for (i = 1; i <= lim; ++i) {
        /* a_i = 2 + 45 * sum_{k=0}^{i-1} a_k */
        coeff[i] = (seq_t)2 + (seq_t)45 * sum;
        sum += coeff[i];
    }

    /* ------------------------------------------------------------------
     * If n ≤ 38, we are done.
     * ------------------------------------------------------------------ */
    if (n <= 38) {
        seq_t result = coeff[n];
        free(coeff);
        return result;
    }

    /* ------------------------------------------------------------------
     * For n ≥ 39, use the full recurrence.
     * sumLast38 holds a_{i-38} + … + a_{i-1} for the current i.
     * ------------------------------------------------------------------ */
    seq_t sumLast38 = sum - coeff[0];   /* sum of a[1] … a[38] */
    for (i = 39; i <= n; ++i) {
        seq_t val = 0;
        /* P_i : 1 when i == 39, else 0 */
        if (i == 39) { val = (seq_t)1; }

        /* val += 45 * (a_{i-1} + … + a_{i-38}) */
        val += (seq_t)45 * sumLast38;

        /* val -= 1035 * a_{i-39} */
        val -= (seq_t)1035 * coeff[i - 39];

        coeff[i] = val;

        /* update sumLast38 for next iteration:
         * remove a_{i-39}, add a_{i-1} */
        sumLast38 = sumLast38 - coeff[i - 39] + coeff[i - 1];
    }

    seq_t result = coeff[n];
    free(coeff);
    return result;
}

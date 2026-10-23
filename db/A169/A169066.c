/*
 * A169066  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1.
   For 1 <= n <= 24:  a(n) = 5*4^(n‑1)
   For n = 25:        a(25) = 3*Σ_{i=1}^{24} a(25 − i) – 6*a(0) + 1
   For n >= 26:       a(n) = 3*Σ_{i=1}^{24} a(n − i) – 6*a(n − 25)  */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate array of n+1 elements */
    size_t len = (size_t)n + 1;
    seq_t *val = (seq_t *)calloc(len, sizeof(seq_t));
    if (!val) return (seq_t)0;          /* should not happen */

    val[0] = (seq_t)1;
    if (n == 0) { seq_t r = val[0]; free(val); return r; }

    /* First 24 terms */
    val[1] = (seq_t)5;
    idx_t up = n < 24 ? n : 24;
    for (idx_t i = 2; i <= up; ++i)
        val[i] = val[i-1] * (seq_t)4;

    if (n >= 25) {
        /* sum of the previous 24 terms (a[1]..a[24]) */
        seq_t sum = (seq_t)0;
        for (idx_t i = 1; i <= 24; ++i)
            sum += val[i];

        /* a[25] uses the special +1 term from the numerator */
        val[25] = (seq_t)3 * sum - (seq_t)6 * val[0] + (seq_t)1;

        /* Update the running sum for subsequent steps */
        sum -= val[1];
        sum += val[25];

        /* Recurrence for n >= 26 */
        for (idx_t k = 26; k <= n; ++k) {
            val[k] = (seq_t)3 * sum - (seq_t)6 * val[k-25];
            sum -= val[k-24];
            sum += val[k];
        }
    }

    seq_t result = val[n];
    free(val);
    return result;
}

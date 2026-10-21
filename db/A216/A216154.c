/*
 * A216154  Triangle read by rows, T(n,k) n>=0, k>=0, generalization of
 *          A000255.
 * https://oeis.org/A216154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(0,0)=1, T(0,k)=0 for k>0. Recurrence: for n>=1, T(n,k) =
 *            T(n-1,k-1) + (1+2*k)*T(n-1,k) + (k+1)*(k+2)*T(n-1,k+1). Sequence
 *            a(n) is obtained by reading the triangle row‑by‑row (row 0
 *            first, then row 1, etc.)
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine the row i and column j that correspond to the global index n. */
    idx_t i = 0, sum = 0;
    while (sum + (i + 1) <= n) {
        sum += (i + 1);
        i++;
    }
    idx_t j = n - sum;

    /* Allocate space for two consecutive rows of the triangle. */
    /* The maximum needed row for n <= 10000 is about 141, so 300 suffices for safety. */
    seq_t prev[300];
    seq_t curr[300];

    /* Initialize row 0. */
    prev[0] = 1;
    for (idx_t k = 1; k <= i; ++k)
        prev[k] = 0;

    /* Build rows 1 .. i. */
    for (idx_t row = 1; row <= i; ++row) {
        for (idx_t k = 0; k <= row; ++k) {
            seq_t term = 0;
            if (k > 0)
                term += prev[k - 1];
            term += prev[k] * ((seq_t)(1 + (seq_t)2 * k));
            if (k + 1 <= row - 1)
                term += prev[k + 1] * ((seq_t)(k + 1)) * ((seq_t)(k + 2));
            curr[k] = term;
        }
        for (idx_t k = 0; k <= row; ++k)
            prev[k] = curr[k];
    }

    return prev[j];
}

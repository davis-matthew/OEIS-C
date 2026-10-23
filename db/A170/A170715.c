/*
 * A170715  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * for 1 ≤ n < 50: * a(n) = 32 * Σ_{i=0}^{n-1} a(i) +
 *            2 * a(50) = 32 * Σ_{i=1}^{49} a(i) - 527 * for n > 50: * a(n) =
 *            32 * Σ_{i=1}^{49} a(n-i) - 528 * a(n-50) * * This recurrence
 *            follows from the rational generating function * given in the
 *            OEIS entry for A170715
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate space for all terms up to n */
    seq_t *term = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!term)
        return (seq_t)0; /* allocation failure – value undefined */

    term[0] = (seq_t)1;
    seq_t sumAll = term[0];

    /* 1 ≤ n < 50 : a(n) = 32 * Σ a(i) + 2 */
    if (n >= 1) {
        for (idx_t i = 1; i <= n && i < 50; ++i) {
            term[i] = (seq_t)32 * sumAll + (seq_t)2;
            sumAll += term[i];
        }
    }

    /* n ≥ 50 : compute a(50) and onward */
    if (n >= 50) {
        /* a(50) = 32 * Σ_{i=1}^{49} a(i) - 527 */
        seq_t sum_1_49 = sumAll - term[0];
        term[50] = (seq_t)32 * sum_1_49 - (seq_t)527;

        /* For n > 50, use the 50‑term recurrence */
        if (n > 50) {
            for (idx_t m = 51; m <= n; ++m) {
                seq_t sum_last49 = (seq_t)0;
                for (idx_t k = 1; k <= 49; ++k)
                    sum_last49 += term[m - k];
                term[m] = (seq_t)32 * sum_last49 - (seq_t)528 * term[m - 50];
            }
        }
    }

    seq_t result = term[n];
    free(term);
    return result;
}

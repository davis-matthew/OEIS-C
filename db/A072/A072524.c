/*
 * A072524  Sum of the remainders when the n-th triangular number is divided
 *          by all smaller triangular numbers > 1.
 * https://oeis.org/A072524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=2}^{n-1} (T_n mod T_k), where T_m = m*(m+1)/2 is the
 *            m-th triangular number. This implementation computes the sum
 *            directly, without any pre‑allocation or external utilities
 * Range:     u8 n <= 16, u16 n <= 100, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* Compute the n-th triangular number once. */
    seq_t tn = ((seq_t)n * (n + (seq_t)1)) / (seq_t)2;

    seq_t sum = (seq_t)0;
    /* T_2 = 3, start with the second triangular number. */
    seq_t tk = (seq_t)3;

    for (idx_t k = 2; k < n; ++k) {
        sum += tn % tk;
        /* Next triangular number: T_{k+1} = T_k + (k+1). */
        tk += (seq_t)(k + (idx_t)1);
    }

    return sum;
}

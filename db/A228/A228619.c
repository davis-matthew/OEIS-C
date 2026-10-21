/*
 * A228619  Number of s in {1,...,n}^n having shortest run with the same value
 *          of length one.
 * https://oeis.org/A228619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n - Σ_{k=1}^{⌊n/2⌋} n * (n-1)^{k-1} * C(n-k-1, k-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0 */

#include "../utils/big_pow.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t max_k = n / 2;          /* floor(n/2) */
    seq_t total_f = 0;            /* Σ ... over all runs ≥ 2 */
    seq_t bin = 1;                /* C(n-k-1, k-1) for current k */
    seq_t pow_nm1 = 1;            /* (n-1)^{k-1} */
    seq_t n_seq = (seq_t)n;
    seq_t n_minus1_seq = (seq_t)(n - 1);

    for (idx_t k = 1; k <= max_k; ++k)
    {
        /* term_k = n * (n-1)^{k-1} * C(n-k-1, k-1) */
        seq_t term = bin * n_seq;
        term *= pow_nm1;
        total_f += term;

        if (k == max_k) break;   /* last iteration */

        /* Update (n-1)^{k} for next loop. */
        pow_nm1 *= n_minus1_seq;

        /* Update bin for k+1 using:
           C(n-(k+1)-1, k) = C(n-k-1, k-1) * (n-2k)(n-2k-1) / ((n-k-1) * k) */
        seq_t num1 = (seq_t)(n - 2 * k);
        seq_t num2 = (seq_t)(n - 2 * k - 1);
        seq_t den1 = (seq_t)(n - k - 1);
        seq_t den2 = (seq_t)(k);
        bin = bin * num1 * num2 / (den1 * den2);
    }

    seq_t total = big_pow(n_seq, n_seq);   /* n^n  */
    return total - total_f;
}

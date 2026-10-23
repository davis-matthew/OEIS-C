/*
 * A038260  Triangle read by rows: T(n,k) = binomial(n,k)*6^(n-k)*6^k,
 *          0<=k<=n.
 * https://oeis.org/A038260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(m, k) * 6^m where m is the row index satisfying
 *            Σ_{i=0}^{m-1} (i+1) ≤ n < Σ_{i=0}^{m} (i+1), and k = n -
 *            Σ_{i=0}^{m-1} (i+1)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Find the row m that contains index n. */
    idx_t m  = 0;
    idx_t cum = 0;               // cumulative count of terms before row m
    while (cum + (m + 1) <= n) {
        cum += m + 1;
        m++;
    }

    /* Position k within row m. */
    idx_t k = n - cum;

    /* Compute binomial coefficient and 6^m. */
    seq_t coeff = binomial(m, k);
    seq_t pow6  = big_pow((seq_t)6, (seq_t)m);

    /* Return their product. */
    return seq_mul(coeff, pow6);
}

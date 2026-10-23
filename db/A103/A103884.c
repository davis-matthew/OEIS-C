/*
 * A103884  Square array A(n,k) read by antidiagonals: row n gives
 *          coordination sequence for lattice C_n.
 * https://oeis.org/A103884
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 60, u32 n <= 198, u64 n <= 720,
 *            u128 n <= 1226, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A103884 – coordination sequences of lattices C_n.
    a(n) is obtained by traversing the square array A(m,k) where
    A(m,0)=1 and

        A(m,k) = Σ_{i=1}^{2k} 2^i * C(m+2, i) * C(2k-1, i-1)

    (C denotes binomial coefficients).  The sequence a is the list of
    these values read by antidiagonals, starting with a(2)=1.
    The traversal order is:
        for each antidiagonal sum s = 0,1,2,...
            for m = s down to 0          // descending row index
                k = s - m
                output A(m+2, k)
*/
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2.  For n < 2 we return 0. */
    if (n < 2) return 0;

    /* Position P in the traversal (1‑based, starting with (m=0,k=0)). */
    idx_t P = n - 1;            /* because a(2) is the first element   */

    /* Find the antidiagonal index m such that
       m(m+1)/2 < P ≤ (m+1)(m+2)/2.    */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 < P) ++m;

    /* Offset inside this antidiagonal (1‑based). */
    idx_t prev_total = m * (m + 1) / 2;
    idx_t offset = P - prev_total;       /* 1 ≤ offset ≤ m+1 */

    /* The coordinates in the array (m' = m - offset + 1,  k = offset - 1). */
    idx_t m_prime = m - offset + 1;       /* 0 ≤ m_prime ≤ m           */
    idx_t k = offset - 1;                /* 0 ≤ k ≤ m                 */

    /* Convert to the original row index n' = m_prime + 2. */
    idx_t n_prime = m_prime + 2;          /* n' = n in the problem   */

    /* Compute A(n_prime, k).  Handle k = 0 separately. */
    if (k == 0) return 1;

    seq_t sum = 0;
    idx_t max_i = (k * 2 < n_prime) ? (k * 2) : n_prime; /* i ≤ n_prime   */
    for (idx_t i = 1; i <= max_i; ++i)
    {
        seq_t bin_n_i   = binom(n_prime, i);
        seq_t bin_2k_1_i_1 = binom(2 * k - 1, i - 1);
        seq_t pow2_i = (seq_t)1 << i;          /* 2^i  */
        sum += pow2_i * bin_n_i * bin_2k_1_i_1;
    }
    return sum;
}

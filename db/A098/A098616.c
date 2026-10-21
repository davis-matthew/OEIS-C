/*
 * A098616  Product of Pell and Catalan numbers: a(n) =
 *          A000129(n+1)*A000108(n).
 * https://oeis.org/A098616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000129(n+1) * A000108(n) (Pell(n+1) multiplied by
 *            Catalan(n))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute Pell(n+1) using P(0)=0, P(1)=1, P(k)=2*P(k-1)+P(k-2) */
    seq_t pell_prev2 = 0;  /* P(k-2) */
    seq_t pell_prev1 = 1;  /* P(k-1) */
    /* Target index is n+1 (always ≥1 for n≥0) */
    if (n + 1 >= 2) {
        for (idx_t i = 2; i <= n + 1; ++i) {
            seq_t pell_cur = 2 * pell_prev1 + pell_prev2;
            pell_prev2 = pell_prev1;
            pell_prev1 = pell_cur;
        }
    }
    seq_t pell_val = pell_prev1;  /* Pell(n+1) */

    /* Compute Catalan(n) using closed form C(n) = binomial(2n, n) / (n+1) */
    seq_t catalan = binomial(2 * n, n);
    catalan = catalan / ((seq_t)n + 1);

    /* Return the product */
    return pell_val * catalan;
}

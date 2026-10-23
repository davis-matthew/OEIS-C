/*
 * A035806  Coordination sequence for lattice D*_42 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (∑_{k=0..42} 2^k * binomial(42, k) * binomial(n-1, k-1))
 *            + 2^42 * binomial(n+20, 41)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 18, u128 n <= 66,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    const idx_t m = 42;
    seq_t sum = 0;

    /* first part of the formula */
    for (idx_t k = 0; k <= m; ++k)
    {
        seq_t term = 0;
        if (k == 0) {
            if (n == 0) term = 1;          /* binomial(-1,-1) = 1 by convention */
        } else if (n >= k) {                  /* binomial(n-1, k-1) is defined only if n-1 >= k-1 */
            seq_t pow2k = ((seq_t)1 << k);
            seq_t cmk   = binomial(m, k);
            seq_t cnk   = binomial(n - 1, k - 1);
            term = pow2k * cmk * cnk;
        }
        sum += term;
    }

    /* second part of the formula */
    seq_t pow2m = ((seq_t)1 << m);           /* 2^42 */
    seq_t cn    = binomial(n + 20, m - 1);   /* binomial(n+20, 41) */
    return sum + pow2m * cn;
}

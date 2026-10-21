/*
 * A203308  a(n) = A203306(n+1)/A203306(n).
 * https://oeis.org/A203308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{j=1..n} ((n+1)! - j!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute (n+1)! */
    seq_t fact_n1 = 1;
    for (idx_t k = 1; k <= n + 1; ++k)
        fact_n1 *= (seq_t)k;

    seq_t product = 1;
    seq_t fac_j   = 1;          /* j! */

    for (idx_t j = 1; j <= n; ++j) {
        fac_j *= (seq_t)j;                      /* fac_j = j!   */
        seq_t term = fact_n1 - fac_j;           /* (n+1)! - j! */
        product *= term;
    }

    return product;
}

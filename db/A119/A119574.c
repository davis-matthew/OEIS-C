/*
 * A119574  a(n) = binomial(2*n,n)*(n+2)^2/(n+1).
 * https://oeis.org/A119574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(2*n, n) * (n+2)^2 / (n+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* compute binomial(2*n, n) */
    seq_t binom = binomial( (idx_t)(2*n), n );

    /* Catalan number C_n = binomial(2*n, n) / (n+1) */
    seq_t catalan = binom / ( (seq_t)(n + 1) );

    /* (n+2)^2 */
    seq_t np2 = (seq_t)(n + 2);
    seq_t val  = catalan * np2 * np2;

    return val;
}

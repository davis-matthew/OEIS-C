/*
 * A119575  a(n) = binomial(2*n,n)*(n+3)^2/(n+1).
 * https://oeis.org/A119575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(2*n, n)*(n+3)^2/(n+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Catalan number C_n = binomial(2*n, n)/(n+1) */
    seq_t res = catalan(n);
    seq_t m   = (seq_t)(n + 3);

    /* a(n) = C_n * (n+3)^2 */
    res = res * m * m;
    return res;
}

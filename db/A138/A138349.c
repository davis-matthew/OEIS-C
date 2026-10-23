/*
 * A138349  Moment sequence of tr(A) in USp(4).
 * https://oeis.org/A138349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if n is odd, otherwise * a(2n) = Catalan(n) *
 *            Catalan(n+2) - Catalan(n+1)^2 * (Catalan numbers A000108 are
 *            given by catalan(k) in utils)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 23, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* If n is odd, the moment is zero. */
    if (n & 1) return (seq_t)0;

    /* Even n: compute using the Catalan formula. */
    idx_t m = n >> 1;            /* m = n/2 */
    seq_t C_m  = catalan(m);
    seq_t C_m1 = catalan(m + 1);
    seq_t C_m2 = catalan(m + 2);

    return C_m * C_m2 - C_m1 * C_m1;
}

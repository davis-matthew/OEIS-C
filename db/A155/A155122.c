/*
 * A155122  a(n) = 4*(3*n+2)*(2*n+1)*(n+2)*(n+1).
 * https://oeis.org/A155122
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(3*n+2)*(2*n+1)*(n+2)*(n+1)
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 114, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Handle terms below the offset */
    if (n < -1)   /* undefined for n < -1, return 0 */
        return 0;
    if (n == -1)  /* a(-1) = 0 */
        return 0;

    /* Convert index to seq_t once */
    seq_t nn = (seq_t)n;

    /* Compute the factors separately to avoid overflow in intermediate values */
    seq_t term1 = 4;
    seq_t term2 = 3 * nn + 2;
    seq_t term3 = 2 * nn + 1;
    seq_t term4 = nn + 2;
    seq_t term5 = nn + 1;

    /* Multiply the factors */
    return term1 * term2 * term3 * term4 * term5;
}

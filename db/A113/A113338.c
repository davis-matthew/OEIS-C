/*
 * A113338  Positive integers of the form (18*m^2+1)/11.
 * https://oeis.org/A113338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (198*n*(n-1) - 81*(2*n-1)*(-1)^n + 83) / 4 * (with offset
 *            1, a(1) = 41)
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 4949, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for the sequence, but safe guard */
        return (seq_t)0;

    /* Compute 198*n*(n-1) safely in seq_t */
    seq_t term1 = (seq_t)198 * (seq_t)n * (seq_t)(n - 1);

    /* Compute 81*(2*n-1) safely in seq_t */
    seq_t term2 = (seq_t)81 * (seq_t)(2 * n - 1);

    /* Add or subtract term2 depending on parity of n */
    if ((n & 1ULL) == 0ULL)        /* n even   → (-1)^n = 1  */
        term1 = term1 - term2 + (seq_t)83;
    else                           /* n odd    → (-1)^n = -1 */
        term1 = term1 + term2 + (seq_t)83;

    /* Final division by 4 (exact) */
    return term1 / (seq_t)4;
}

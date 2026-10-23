/*
 * A121013  Denominators of partial alternating sums of Catalan numbers scaled
 *          by powers of 1/(11^2) = 1/121.
 * https://oeis.org/A121013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 11^(2n) for n≠6,7,8,9 a(n) = 11^(2n−1) for n=6,7,8,9
 *            (OEIS A121013 – denominators of the partial alternating sums of
 *            Catalan numbers scaled by powers of 1/(11^2))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t once for arithmetic. */
    seq_t sn = (seq_t)n;
    seq_t exp;

    if (n <= 5) {
        exp = sn * 2;            /* exponent 2n */
    } else if (n <= 9) {
        exp = sn * 2 - 1;        /* exponent 2n−1 for n=6…9 */
    } else {
        exp = sn * 2;            /* back to exponent 2n */
    }

    return big_pow((seq_t)11, exp);
}

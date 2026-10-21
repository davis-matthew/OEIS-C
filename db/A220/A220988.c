/*
 * A220988  The right Aurifeuillian factor of 11^(22n+11) + 1.
 * https://oeis.org/A220988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 161051 z^10 + 161051 z^9 + 73205 z^8 + 14641 z^7 * - 1331
 *            z^6 - 1331 z^5 - 121 z^4 + 121 z^3 + 55 z^2 + 11 z + 1 * with z
 *            = 11^n
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 1, u128 n <= 3,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* z = 11^n */
    seq_t z = pow_int((seq_t)11, n);

    /* Horner evaluation */
    seq_t res = (seq_t)161051;        /* c10 */
    res = res * z + (seq_t)161051;    /* + c9 */
    res = res * z + (seq_t)73205;     /* + c8 */
    res = res * z + (seq_t)14641;     /* + c7 */
    res = res * z - (seq_t)1331;      /* - c6 */
    res = res * z - (seq_t)1331;      /* - c5 */
    res = res * z - (seq_t)121;       /* - c4 */
    res = res * z + (seq_t)121;       /* + c3 */
    res = res * z + (seq_t)55;        /* + c2 */
    res = res * z + (seq_t)11;        /* + c1 */
    res = res * z + (seq_t)1;         /* + c0 */

    return res;
}

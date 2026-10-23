/*
 * A022151  Coordination sequence for root lattice B_9.
 * https://oeis.org/A022151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2012*n^8 - 3952*n^7 + 27160*n^6 - 35392*n^5 + 81508*n^4
 *            - 57568*n^3 + 45560*n^2 - 8928*n + 630) / 315
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 12, u64 n <= 203, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Coefficients for the polynomial in descending exponent order
       (n^7 down to the constant term). */
    static const int coeffs[8] = {
        -3952,  27160,  -35392,  81508,
        -57568, 45560,   -8928,    630
    };

    /* Use a signed 128‑bit integer for intermediate calculation.
       The expression is guaranteed to fit in 128 bits for all
       n up to at least 10000 (the typical test range). */
    __int128 num = ( __int128 )2012;          /* coefficient of n^8 */

    for (int i = 0; i < 8; ++i) {
        num = num * ( __int128 )n + ( __int128 )coeffs[i];
    }

    num /= ( __int128 )315;   /* Exact division */

    return (seq_t)num;
}

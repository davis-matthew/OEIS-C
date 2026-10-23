/*
 * A037545  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,2.
 * https://oeis.org/A037545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(26 * 4^n / 63)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q = 0;            /* quotient part of 26*4^i / 63 */
    uint64_t r = 26 % 63;   /* remainder part, always < 63 */

    for (idx_t i = 1; i <= n; ++i) {
        uint64_t r4 = r << 2;                    /* r * 4  (since r < 63, r4 < 252) */
        seq_t add = (seq_t)(r4 / 63);            /* floor(4 * r / 63)   */
        q = q * 4 + add;                         /* q_i = 4*q_{i-1} + add */
        r = r4 % 63;                             /* remainder for next step */
    }
    return q;                                    /* a(n) */
}

/*
 * A219394  Numbers k such that 17*k+1 is a square.
 * https://oeis.org/A219394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (34*n*(n-1)+13*(-1)^n*(2*n-1)+5)/8 + 1
 * Range:     u8 n <= 7, u16 n <= 125, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use signed 128‑bit arithmetic to avoid overflow for very large n */
    __int128 nn = (__int128)n;
    __int128 num;

    if ((nn & 1) == 0) /* n even */
        num = 34 * nn * (nn - 1) + 13 * (2 * nn - 1) + 5;
    else                /* n odd */
        num = 34 * nn * (nn - 1) - 13 * (2 * nn - 1) + 5;

    num = num / 8 + 1;
    return (seq_t)num;
}

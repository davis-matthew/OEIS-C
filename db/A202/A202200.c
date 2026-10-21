/*
 * A202200  Number of (n+2) X 8 binary arrays avoiding patterns 001 and 101 in
 *          rows and columns.
 * https://oeis.org/A202200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+8)*(n+7)*(n+6)*(n+5)*(n+4)*(n+3)*(n+2)^2/315
 * Range:     u8 none, u16 n <= 3, u32 n <= 28, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* value outside the defined offset */
    unsigned __int128 x = n;
    unsigned __int128 r = (x + 8);
    r *= (x + 7);
    r *= (x + 6);
    r *= (x + 5);
    r *= (x + 4);
    r *= (x + 3);
    r *= (x + 2);
    r *= (x + 2);
    r /= 315;                            /* division is exact by the formula */
    return (seq_t)r;
}

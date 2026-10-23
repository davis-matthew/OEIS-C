/*
 * A161711  a(n) = (-4*n^3 + 27*n^2 - 20*n + 3)/3.
 * https://oeis.org/A161711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-4*n^3 + 27*n^2 - 20*n + 3)/3
 * Range:     i8 n <= 7, i16 n <= 31, i32 n <= 1174, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate to avoid overflow when n is large */
    __int128 nn = ( __int128 ) n;
    __int128 n2 = nn * nn;
    __int128 n3 = n2 * nn;
    __int128 num = -4 * n3 + 27 * n2 - 20 * nn + 3;
    return (seq_t)(num / 3);
}

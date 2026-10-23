/*
 * A122658  a(n) = if n mod 2 = 1 then n^3*(n-1)^2/2 else n^5/2.
 * https://oeis.org/A122658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n mod 2 = 1 then n^3*(n-1)^2/2 else n^5/2
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1) == 1) {                 /* odd n */
        seq_t n1 = nn - 1;
        return (nn * nn * nn * n1 * n1) / 2;
    } else {                            /* even n */
        return (nn * nn * nn * nn * nn) / 2;
    }
}

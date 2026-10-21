/*
 * A086760  a(n) = 8*n^2 + 88*n + 43.
 * https://oeis.org/A086760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^2 + 88*n + 43
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;            /* cast index to the proper type first */
    seq_t term = sn * sn;           /* n^2        */
    term = term * 8;                /* 8*n^2      */
    term += sn * 88;                /* +88*n      */
    term += (seq_t)43;              /* +43        */
    return term;
}

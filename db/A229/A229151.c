/*
 * A229151  a(n) = n^8*(5*n+4).
 * https://oeis.org/A229151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8*(5*n+4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 115, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          // n
    seq_t n2 = nn * nn;           // n^2
    seq_t n4 = n2 * n2;           // n^4
    seq_t n8 = n4 * n4;           // n^8
    seq_t five_n_plus4 = 5 * nn + 4; // 5*n + 4
    return n8 * five_n_plus4;
}

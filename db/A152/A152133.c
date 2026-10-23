/*
 * A152133  Maximal length of rook tour on an n X n+2 board.
 * https://oeis.org/A152133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 12*n^2 + 2*n + 9 + 3*(-1)^n)/6
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;        /* n^2   */
    seq_t n3 = n2 * n_seq;           /* n^3   */
    seq_t sum = 4 * n3 + 12 * n2 + 2 * n_seq + 9;
    if (n & 1)                       /* n odd  -> add (-3) */
        sum -= 3;
    else                             /* n even -> add (+3) */
        sum += 3;
    return sum / 6;
}

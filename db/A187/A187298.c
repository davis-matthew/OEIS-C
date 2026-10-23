/*
 * A187298  Number of 3-step one space leftwards or up, two space rightwards
 *          or down asymmetric rook's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A187298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*n^2 - 44*n + 18 for n>3; a(1)=0, a(2)=2, a(3)=36
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 2;
    if (n == 3) return 36;
    seq_t N = (seq_t)n;
    return (seq_t)16 * N * N - (seq_t)44 * N + (seq_t)18;
}

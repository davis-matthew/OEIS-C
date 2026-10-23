/*
 * A187164  Number of 3-step self-avoiding walks on an n X n X n cube summed
 *          over all starting positions.
 * https://oeis.org/A187164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; a(n) = 30*n^3 - 60*n^2 + 24*n for n>1
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    return 30 * n3 - 60 * n2 + 24 * nn;
}

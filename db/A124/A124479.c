/*
 * A124479  From the game of Quod: number of "squares" on an n X n array of
 *          points with the four corner points deleted.
 * https://oeis.org/A124479
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - n^2 - 48*n + 84)/12
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;
    seq_t val = n4 - n2 - (seq_t)48 * nn + (seq_t)84;
    return val / (seq_t)12;
}

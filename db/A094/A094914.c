/*
 * A094914  Absolute value of n^2 + n - 1354363.
 * https://oeis.org/A094914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |n^2 + n - 1354363|
 * Range:     u8 none, u16 none, u32 n <= 2000, u64 n <= 2000, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn + 1);
    const seq_t C = 1354363;
    if (prod >= C)
        return prod - C;
    else
        return C - prod;
}

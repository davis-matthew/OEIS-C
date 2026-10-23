/*
 * A157844  a(n) = 103680000*n^2 - 161251200*n + 62697601.
 * https://oeis.org/A157844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 103680000*n^2 - 161251200*n + 62697601
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t t1   = (seq_t)103680000 * nn * nn;
    seq_t t2   = (seq_t)161251200 * nn;
    return t1 - t2 + (seq_t)62697601;
}

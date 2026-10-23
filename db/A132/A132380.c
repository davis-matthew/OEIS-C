/*
 * A132380  Period 8: repeat [0, 0, 1, 1, 0, 0, -1, -1].
 * https://oeis.org/A132380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = period 8: [0,0,1,1,0,0,-1,-1] for all n ≥ 0
 * Range:     i8 n <= 8191, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[8] = {0, 0, 1, 1, 0, 0, -1, -1};
    return tbl[n & 7];
}

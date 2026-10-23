/*
 * A173391  a(n) = 6n + 3^n.
 * https://oeis.org/A173391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n + 3^n
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow3 = 1;
    for(idx_t i = 0; i < n; ++i)
        pow3 = pow3 * 3;
    return (seq_t)6 * n + pow3;
}

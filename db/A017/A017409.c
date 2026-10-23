/*
 * A017409  a(n) = (11n+1)^9.
 * https://oeis.org/A017409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11 n + 1)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)11 * n) + 1;
    seq_t res  = 1;
    for (int i = 0; i < 9; ++i)
        res *= base;
    return res;
}

/*
 * A165283  a(n) = (2*n + 1)*16^n.
 * https://oeis.org/A165283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 1)*16^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p = 1;
    for (idx_t i = 0; i < n; ++i)
        p *= (seq_t)16;
    seq_t two_n_plus_one = (seq_t)2 * (seq_t)n + 1;
    return p * two_n_plus_one;
}

/*
 * A173200  Solutions y of the Mordell equation y^2 = x^3 - 3a^2 - 1 for a =
 *          0,1,2, ... (solutions x are given by A053755).
 * https://oeis.org/A173200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^3 - 24*n^2 + 27*n - 11
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 813, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t term = 8 * n3 - 24 * n2 + 27 * nn - (seq_t)11;
    return term;
}

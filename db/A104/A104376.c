/*
 * A104376  a(n) = Sum_{j=0..13} n^j.
 * https://oeis.org/A104376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n + n^2 + ... + n^13
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 30, u128 n <= 920,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t term   = 1;
    seq_t N      = (seq_t)n;
    for (idx_t j = 1; j <= 13; ++j) {
        term *= N;
        result += term;
    }
    return result;
}

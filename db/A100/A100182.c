/*
 * A100182  Structured tetragonal anti-prism numbers.
 * https://oeis.org/A100182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 1543, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (7*n^3 - 3*n^2 + 2*n)/6
 */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t num = 7 * n3 - 3 * n2 + 2 * nn;
    return num / 6;
}

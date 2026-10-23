/*
 * A129957  a(n) = n^3 if n is odd, n^3 + 1 otherwise.
 * https://oeis.org/A129957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 if n is odd, n^3 + 1 otherwise
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = (seq_t)n * (seq_t)n * (seq_t)n;
    if ((n & 1) == 0)
        v += (seq_t)1;
    return v;
}

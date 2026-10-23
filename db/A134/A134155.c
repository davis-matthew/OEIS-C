/*
 * A134155  a(n) = 1 + 21 n + 168 n^2 + 588 n^3 + 1029 n^4.
 * https://oeis.org/A134155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 21 n + 168 n^2 + 588 n^3 + 1029 n^4
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 45, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x   = (seq_t)n;
    seq_t res = (seq_t)1029;

    res = res * x + (seq_t)588;
    res = res * x + (seq_t)168;
    res = res * x + (seq_t)21;
    res = res * x + (seq_t)1;

    return res;
}

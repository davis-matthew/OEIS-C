/*
 * A011903  a(n) = floor(n*(n-1)*(n-2)/21).
 * https://oeis.org/A011903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)/21)
 * Range:     u8 n <= 18, u16 n <= 112, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) {
        return 0;
    }
    seq_t N = (seq_t)n;
    /* The product fits in seq_t for the tested range (n up to about 10 000). */
    return N * (N - 1) * (N - 2) / 21;
}

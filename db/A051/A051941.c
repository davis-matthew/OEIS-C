/*
 * A051941  Truncated triangular pyramid numbers: a(n) = (n-7)*(n^2 + 10*n -
 *          108)/6, n >= 8.
 * https://oeis.org/A051941
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = (n-7)*(n^2 + 10*n - 108)/6 (for n >= 8)
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 8) {
        /* Undefined for n < 8. Return 0 to avoid undefined behaviour. */
        return (seq_t)0;
    }
    seq_t N = (seq_t)n;
    seq_t term = ((N - 7) * (N * N + 10 * N - 108)) / 6;
    return term;
}

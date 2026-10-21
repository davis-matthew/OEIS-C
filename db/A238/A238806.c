/*
 * A238806  Number of n X 2 0..2 arrays with no element equal to one plus the
 *          sum of elements to its left or one plus the sum of the sum of
 *          elements above it, modulo 3.
 * https://oeis.org/A238806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 23*n - 6) / 6
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour for n < 1 is irrelevant */
    seq_t N  = (seq_t)n;
    seq_t num = N * N * N + (seq_t)23 * N - (seq_t)6;
    return num / (seq_t)6;
}

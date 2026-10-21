/*
 * A051942  a(n) = n*(n+1)/2 - 45.
 * https://oeis.org/A051942
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = n*(n+1)/2 - 45
 * Range:     u8 n <= 24, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using seq_t to avoid overflow of intermediate conversions. */
    seq_t tn  = (seq_t)n;
    seq_t val = tn * (tn + 1) / 2;
    val -= 45;
    return val;
}

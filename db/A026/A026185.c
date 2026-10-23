/*
 * A026185  If n even, then 2n. If n odd, then nearest integer to 2n/3.
 * https://oeis.org/A026185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   If n is even, a(n) = 2*n; if n is odd, a(n) = (2*n + 1) / 3
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0)
        return ((seq_t)n * 2);
    else
        return (((seq_t)n * 2 + 1) / 3);
}

/*
 * A168336  a(n) = 5 + 7*floor((n-1)/2).
 * https://oeis.org/A168336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 5 + 7*floor((n-1)/2)
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* assume n >= 1 (offset is 1) */
    seq_t half = ((seq_t)n - (seq_t)1) / (seq_t)2;
    return (seq_t)5 + (seq_t)7 * half;
}

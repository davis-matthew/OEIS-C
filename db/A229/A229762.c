/*
 * A229762  a(n) = (n XOR floor(n/2)) AND floor(n/2), where AND and XOR are
 *          bitwise logical operators.
 * https://oeis.org/A229762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n XOR floor(n/2)) AND floor(n/2)
 * Range:     u8 n <= 511, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t half = (seq_t)(n >> 1);   /* floor(n/2) */
    return (((seq_t)n) ^ half) & half;
}

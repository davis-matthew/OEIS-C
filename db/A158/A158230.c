/*
 * A158230  a(n) = 256*n^2 + 2*n.
 * https://oeis.org/A158230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 256*n^2 + 2*n (offset 1)
 * Range:     u8 none, u16 n <= 15, u32 n <= 4095, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;          /* promote index to seq_t */
    const seq_t C256 = (seq_t)256;
    const seq_t TWO  = (seq_t)2;

    /* Compute 256*n^2 + 2*n */
    seq_t result = tn * (C256 * tn + TWO);
    return result;
}

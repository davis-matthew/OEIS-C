/*
 * A140680  a(n) = n*(3*n+16).
 * https://oeis.org/A140680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n + 16)
 * Range:     u8 n <= 6, u16 n <= 145, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* Convert index to the sequence type first */
    return nn * ((seq_t)3 * nn + (seq_t)16);
}

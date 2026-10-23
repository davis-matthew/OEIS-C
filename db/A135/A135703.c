/*
 * A135703  a(n) = n*(7*n-2).
 * https://oeis.org/A135703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (7 * n - 2)
 * Range:     u8 n <= 6, u16 n <= 96, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the result type before arithmetic */
    seq_t nn = (seq_t)n;
    return nn * ((seq_t)7 * nn - (seq_t)2);
}

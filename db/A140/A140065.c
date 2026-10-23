/*
 * A140065  a(n) = (7*n^2 - 17*n + 12)/2.
 * https://oeis.org/A140065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n^2 - 17*n + 12)/2
 * Range:     u8 n <= 7, u16 n <= 133, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t once to keep the calculation type consistent. */
    seq_t sn = (seq_t)n;
    /* Compute the numerator: 7*n^2 - 17*n + 12. */
    seq_t numerator = sn * ((seq_t)7 * sn - (seq_t)17) + (seq_t)12;
    /* The numerator is always even; perform the exact integer division. */
    return numerator / (seq_t)2;
}

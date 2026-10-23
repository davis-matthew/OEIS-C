/*
 * A016746  a(n) = (2*n)^6.
 * https://oeis.org/A016746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n)^6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 812,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t twon = ((seq_t)n) * (seq_t)2;
    /* compute the sixth power in a single expression */
    seq_t res = twon * twon * twon * twon * twon * twon;
    return res;
}

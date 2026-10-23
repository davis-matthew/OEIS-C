/*
 * A016913  a(n) = (6*n)^5.
 * https://oeis.org/A016913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n)^5
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 14, u64 n <= 1188,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type first. */
    seq_t nn = (seq_t)n;
    /* Compute 7776 * n^5 as a sequence of 64‑bit multiplications. */
    seq_t res = (seq_t)7776;
    res *= nn;
    res *= nn;
    res *= nn;
    res *= nn;
    res *= nn;
    return res;
}

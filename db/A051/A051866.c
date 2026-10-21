/*
 * A051866  14-gonal (or tetradecagonal) numbers: a(n) = n*(6*n-5).
 * https://oeis.org/A051866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(6*n-5)
 * Range:     u8 n <= 6, u16 n <= 104, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the sequence type */
    seq_t nn = (seq_t)n;
    /* Compute the 14‑gonal (tetradecagonal) number */
    return nn * ((nn * 6) - 5);
}

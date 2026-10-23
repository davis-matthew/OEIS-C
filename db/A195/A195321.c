/*
 * A195321  a(n) = 18*n^2.
 * https://oeis.org/A195321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*n^2
 * Range:     u8 n <= 3, u16 n <= 60, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before arithmetic to avoid
       accidental 64‑bit overflow on the left‑hand side when n is large. */
    seq_t nn = (seq_t)n;
    return nn * nn * 18;
}

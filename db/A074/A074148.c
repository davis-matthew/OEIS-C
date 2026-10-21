/*
 * A074148  a(n) = n + floor(n^2/2).
 * https://oeis.org/A074148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n^2/2)
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before arithmetic to avoid narrowing. */
    seq_t nn = (seq_t)n;
    /* Compute floor(n^2/2) by integer division. */
    return nn + (nn * nn) / 2;
}

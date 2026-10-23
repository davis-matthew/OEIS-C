/*
 * A177099  a(n) = 81*n^2 + 2*n.
 * https://oeis.org/A177099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 81*n^2 + 2*n
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 7281, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is unsigned 64‑bit; compute in seq_t to use the chosen precision. */
    seq_t nn = (seq_t)n;
    seq_t term = (seq_t)81 * nn + (seq_t)2;  /* 81*n + 2  */
    return nn * term;                       /* n*(81*n + 2) */
}

/*
 * A152948  a(n) = (n^2 - 3*n + 6)/2.
 * https://oeis.org/A152948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - 3*n + 6)/2
 * Range:     u8 n <= 24, u16 n <= 363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rn = (seq_t)n;                /* cast index to seq_t first */
    return (rn * rn - 3 * rn + 6) / 2;  /* compute the closed‑form formula */
}

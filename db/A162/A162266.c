/*
 * A162266  a(n) = (2*n^3 + 5*n^2 + 21*n)/2.
 * https://oeis.org/A162266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 5*n^2 + 21*n)/2
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    /* compute 2*n^3 + 5*n^2 + 21*n, then divide by 2 */
    seq_t val = ((seq_t)2 * sn * sn * sn
                + (seq_t)5 * sn * sn
                + (seq_t)21 * sn) / (seq_t)2;
    return val;
}

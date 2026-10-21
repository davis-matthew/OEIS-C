/*
 * A064487  Order of twisted Suzuki group Sz(2^(2*n + 1)), also known as the
 *          group 2B2(2^(2*n + 1)).
 * https://oeis.org/A064487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = m^2 * (m - 1) * (m^2 + 1), where m = 2^(2*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute m = 2^(2*n+1) */
    idx_t shift = 2*n + 1;
    seq_t m = ((seq_t)1 << shift);
    seq_t m2 = m * m;          /* m^2                     */
    seq_t part = m2 * (m - 1); /* m^2 * (m-1)             */
    return part * (m2 + 1);    /* we now multiply by (m^2+1) */
}

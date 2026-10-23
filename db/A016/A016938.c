/*
 * A016938  a(n) = (6*n + 2)^6.
 * https://oeis.org/A016938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 2)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 6, u64 n <= 270, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute m = 6*n + 2 */
    seq_t m = (seq_t)6 * (seq_t)n + (seq_t)2;

    /* Compute powers: m^2, m^4, then m^6 */
    seq_t m2 = m * m;      /* m^2 */
    seq_t m4 = m2 * m2;    /* m^4 */
    seq_t res = m4 * m2;   /* m^6 */

    return res;
}

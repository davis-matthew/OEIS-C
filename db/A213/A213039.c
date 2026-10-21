/*
 * A213039  a(n) = n^3 - floor(n/3)^3.
 * https://oeis.org/A213039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 - (floor(n/3))^3
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t m  = sn / (seq_t)3;          /* floor(n/3) */
    seq_t sn2 = sn * sn;               /* n^2 */
    seq_t sn3 = sn2 * sn;              /* n^3 */
    seq_t m2  = m * m;                 /* m^2 */
    seq_t m3  = m2 * m;                /* m^3 */
    return sn3 - m3;
}

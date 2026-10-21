/*
 * A226741  Column 4 of array in A226513.
 * https://oeis.org/A226741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)^4 + 12*(n + 1)^3 + 36*(n + 1)^2 + 26*(n + 1)
 * Range:     u8 n <= 0, u16 n <= 12, u32 n <= 252, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n + 1;          /* m = n + 1 */
    seq_t m2 = m * m;                /* m^2 */
    seq_t m3 = m2 * m;               /* m^3 */
    seq_t m4 = m3 * m;               /* m^4 */
    return m4 + 12 * m3 + 36 * m2 + 26 * m;
}

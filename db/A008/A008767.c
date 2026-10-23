/*
 * A008767  a(n) = floor(n/7)*ceiling(n/7).
 * https://oeis.org/A008767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/7) * ceil(n/7)
 * Range:     u8 n <= 111, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor division */
    seq_t low  = (seq_t)(n / 7);
    /* ceil division: (n + 6) / 7 */
    seq_t high = (seq_t)((n + 6) / 7);
    return low * high;
}

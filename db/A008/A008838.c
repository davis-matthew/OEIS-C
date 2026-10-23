/*
 * A008838  a(n) = floor(n/8)*ceiling(n/8).
 * https://oeis.org/A008838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/8)*ceil(n/8)
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute floor(n/8) and ceil(n/8) as idx_t, then convert to seq_t */
    seq_t q = (seq_t)(n / 8);          /* floor division */
    seq_t r = (seq_t)((n + 7) / 8);    /* ceil division */
    return q * r;
}

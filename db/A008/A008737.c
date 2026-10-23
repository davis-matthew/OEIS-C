/*
 * A008737  a(n) = floor(n/6)*ceiling(n/6).
 * https://oeis.org/A008737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/6)*ceil(n/6)
 * Range:     u8 n <= 95, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute floor(n/6) and ceil(n/6) safely for any uint64_t n */
    idx_t f = n / 6;                     /* floor(n/6) */
    idx_t r = n % 6;                     /* remainder of n/6 */
    idx_t c = f + (r != 0 ? 1 : 0);      /* ceil(n/6) */
    return (seq_t)f * (seq_t)c;          /* product in seq_t */
}

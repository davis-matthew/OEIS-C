/*
 * A038715  a(n) = floor(n/4)*ceiling((n+2)/4).
 * https://oeis.org/A038715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/4)*ceil((n+2)/4)
 * Range:     u8 n <= 63, u16 n <= 1023, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 77 terms through n = 9683 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;            /* promote idx_t to seq_t first */
    seq_t floor_part = N / 4;          /* floor(n/4)   */
    seq_t ceil_part  = (N + 5) / 4;    /* ceil((n+2)/4) = (n+5)/4 */
    return floor_part * ceil_part;
}

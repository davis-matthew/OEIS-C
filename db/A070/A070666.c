/*
 * A070666  n^6 mod 43.
 * https://oeis.org/A070666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 43
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n % 43;        /* reduce n modulo 43 */
    seq_t t = (m * m) % 43;        /* m^2 */
    t = (t * t) % 43;              /* m^4 */
    t = (t * m) % 43;              /* m^5 */
    t = (t * m) % 43;              /* m^6 */
    return t;
}

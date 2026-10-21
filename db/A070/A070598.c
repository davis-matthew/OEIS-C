/*
 * A070598  n^5 mod 13.
 * https://oeis.org/A070598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 13
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = (seq_t)(n % 13);          /* base reduced mod 13 */
    seq_t v = (r * r) % 13;             /* r^2 mod 13 */
    v = (v * v) % 13;                  /* r^4 mod 13 */
    v = (v * r) % 13;                  /* r^5 mod 13 */
    return v;
}

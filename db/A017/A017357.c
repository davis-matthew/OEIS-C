/*
 * A017357  a(n) = (10*n + 7)^5.
 * https://oeis.org/A017357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 7)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 7, u64 n <= 712, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = ((seq_t)10 * (seq_t)n) + (seq_t)7;   /* t = 10*n + 7 */
    seq_t r = t * t;     /* r = t²   */
    r = r * r;           /* r = t⁴   */
    r = r * t;           /* r = t⁵   */
    return r;
}

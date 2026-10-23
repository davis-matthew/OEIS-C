/*
 * A153509  Period 9: repeat [6, 6, 6, 3, 3, 3, 0, 0, 0].
 * https://oeis.org/A153509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*((3 - floor((n+3)/3)) mod 3)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 9;                    /* period 9 */
    if (r < 3) return (seq_t)6;          /* [6,6,6]   */
    if (r < 6) return (seq_t)3;          /* [3,3,3]   */
    return (seq_t)0;                     /* [0,0,0]   */
}

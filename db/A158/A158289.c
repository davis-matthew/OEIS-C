/*
 * A158289  Period 18 zigzag sequence: repeat
 *          [0,1,2,3,4,5,6,7,8,9,8,7,6,5,4,3,2,1].
 * https://oeis.org/A158289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = abs(n - 18*round(n/18)) Equivalent to the period‑18
 *            zigzag: a(n) = n % 18 <= 9 ? n%18 : 18-(n%18)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 18;      /* 0 ≤ r ≤ 17 */
    if (r <= 9)
        return (seq_t)r;   /* ascending part 0…9 */
    else
        return (seq_t)(18 - r);  /* descending part 8…1 */
}

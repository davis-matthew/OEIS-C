/*
 * A186187  Period 8 sequence [ 2, 2, 1, 2, 4, 2, 1, 2, ...] except a(0) = 1.
 * https://oeis.org/A186187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0, a(n)=4 if n%8==4, a(n)=1 if n%4==2, otherwise 2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    idx_t r = n & 7;          /* r = n % 8 */
    if (r == 2 || r == 6)     /* n % 4 == 2 */
        return 1;
    if (r == 4)               /* n % 8 == 4 */
        return 4;
    return 2;
}

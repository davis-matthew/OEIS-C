/*
 * A091829  a(1)=1; a(2n)=a(n)+1, a(2n+1)=a(n) mod 2.
 * https://oeis.org/A091829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(2n)=a(n)+1; a(2n+1)=a(n) mod 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n & 1)      /* odd: n = 2k+1, k = n>>1 */
        return a(n >> 1) % 2;
    else            /* even: n = 2k   */
        return a(n >> 1) + 1;
}

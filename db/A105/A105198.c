/*
 * A105198  a(n) = n(n+1)/2 mod 4.
 * https://oeis.org/A105198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 mod 4
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t mask = 7;                     /* 2^3 - 1 = 8, reduces n modulo 8 */
    idx_t m  = n & mask;                      /* n mod 8 */
    idx_t mp = (n + 1) & mask;                /* (n+1) mod 8 */
    idx_t prod = (m * mp) & mask;             /* n*(n+1) mod 8, always even */
    return (seq_t)(prod >> 1);                /* divide by 2 ; result 0..3 */
}

/*
 * A110549  Period 8: repeat [1, 2, 4, 3, 3, 4, 2, 1].
 * https://oeis.org/A110549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (n*(n+1)/2 mod 4)
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n(n+1)/2 modulo 4 can be obtained from (n mod 8) * ((n+1) mod 8) modulo 8,
       then halved.  This avoids any overflow. */
    const idx_t mask8 = 7ULL;                       /* 8‑1 */
    idx_t n8   = n & mask8;                         /* n mod 8 */
    idx_t n1   = (n8 + 1) & mask8;                  /* (n+1) mod 8 */
    idx_t prod = (n8 * n1) & mask8;                 /* n(n+1) mod 8  */
    idx_t tri  = prod >> 1;                         /* (n(n+1)/2) mod 4 */
    return (seq_t)(tri + 1);
}

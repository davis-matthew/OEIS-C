/*
 * A133882  a(n) = binomial(n+2,n) mod 2^2.
 * https://oeis.org/A133882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+2,2) mod 4 (= ((n+1)*(n+2)/2) mod 4)
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work modulo 8 to avoid overflow, then divide by 2. */
    idx_t r1 = (n + 1) & 7;   /* (n+1) mod 8 */
    idx_t r2 = (n + 2) & 7;   /* (n+2) mod 8 */
    idx_t prod = (r1 * r2) & 7; /* product mod 8, always even */
    return (seq_t)(prod >> 1); /* (product/2) mod 4 */
}

/*
 * A008728  Molien series for 3-dimensional group [2,n ] = *22n.
 * https://oeis.org/A008728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n/10+1) * (5*(n/10+1) + (n%10) - 4)
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q_idx = n / 10 + 1;          /* q = floor((n+10)/10) = floor(n/10)+1 */
    seq_t q = (seq_t)q_idx;
    seq_t r = (seq_t)(n % 10);        /* r = n mod 10 */
    return q * (5 * q + r - 4);
}

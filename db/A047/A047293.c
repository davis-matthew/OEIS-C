/*
 * A047293  Numbers that are congruent to {0, 2, 4, 6} mod 7.
 * https://oeis.org/A047293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*((n-1)/4) + 2*((n-1)%4)
 * Range:     u8 n <= 146, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to seq_t once to keep all arithmetic in seq_t */
    seq_t m = (seq_t)n - 1;      /* m = n-1 */
    seq_t q = m >> 2;            /* q = floor((n-1)/4) */
    seq_t r = m & 3;             /* r = (n-1) mod 4 */

    /* a(n) = 7*q + 2*r  ->  (q<<2)+(q<<1)+q + (r<<1) */
    return (q << 2) + (q << 1) + q + (r << 1);
}

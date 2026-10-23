/*
 * A124072  First differences of A129819.
 * https://oeis.org/A124072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n%2==0 ? n/4 : (n+1)/2)
 * Range:     u8 n <= 510, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently: a(2m) = floor(m/2),  a(2m+1) = m+1 */

seq_t a(idx_t n)
{
    /* if n is odd, return m+1 where n = 2m+1 */
    if (n & 1u) {
        return (seq_t)((n >> 1u) + 1u);
    }
    /* if n is even, return floor(n/4) */
    return (seq_t)(n >> 2u);
}

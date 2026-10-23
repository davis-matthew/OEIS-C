/*
 * A131174  a(2n) = 2*A000217(n), a(2n+1) = A000217(n).
 * https://oeis.org/A131174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = 2*A000217(n), a(2n+1) = A000217(n)
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the efficient integer expressions derived from the definition. */
seq_t a(idx_t n)
{
    seq_t result;
    if ((n & 1) == 0)   /* n is even: a(n) = (n/2) * ((n/2)+1) */
    {
        seq_t m = (seq_t)(n / 2);
        result = m * (m + 1);
    }
    else   /* n is odd: a(n) = ((n-1)/2) * (((n-1)/2)+1) / 2 */
    {
        seq_t k = (seq_t)((n - 1) / 2);
        result = (k * (k + 1)) / 2;
    }
    return result;
}

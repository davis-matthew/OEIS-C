/*
 * A089013  a(n) = (A088567(8n) mod 2).
 * https://oeis.org/A089013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(2*n) = a(n) (n > 0); a(2*n+1) = n mod 2 (n ≥ 0)
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* remove all trailing zero bits – obtain the odd part of n */
    idx_t m = n;
    while ((m & 1) == 0)
        m >>= 1;

    /* for odd m, a(n) = ((m-1)/2) mod 2 */
    seq_t k = ((seq_t)m - 1) >> 1;
    return k & 1;
}

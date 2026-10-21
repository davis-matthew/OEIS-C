/*
 * A226107  Number of strict partitions of n with Cookie Monster number 2.
 * https://oeis.org/A226107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n is odd) (n-1)/2 * | (n is even) (n/2 - 1 + floor((n/2
 *            - 1)/2))
 * Range:     u8 n <= 342, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1U) /* odd */
        return ((seq_t)(n - 1U)) / 2U;
    else        /* even */
    {
        idx_t m = n / 2U;
        return ((seq_t)(m - 1U)) + ((seq_t)(m - 1U) / 2U);
    }
}

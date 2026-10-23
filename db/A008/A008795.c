/*
 * A008795  Molien series for 3-dimensional representation of dihedral group
 *          D_6 of order 6.
 * https://oeis.org/A008795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n/2+2, 2) when n is even, binomial((n+1)/2, 2)
 *            when n is odd
 * Range:     u8 n <= 43, u16 n <= 721, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((nn & 1) == 0)          /* n is even */
    {
        seq_t k = nn / 2;       /* k = n/2 */
        return ((k + 2) * (k + 1)) / 2; /* binomial(k+2, 2) */
    }
    else                        /* n is odd */
    {
        seq_t k = (nn + 1) / 2; /* k = (n+1)/2, k >= 1 */
        return (k * (k - 1)) / 2; /* binomial(k, 2) */
    }
}

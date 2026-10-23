/*
 * A026002  a(n) = T(n,n+2), where T = Delannoy triangle (A008288).
 * https://oeis.org/A026002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0}^{n-1} binomial(n-1, k)*binomial(n+1, k)*2^k
 *            (offset 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) /* sequence offset is 1, no defined term for n=0 */
        return 0;

    seq_t sum     = 0;
    seq_t c1      = 1;          /* C(n-1, 0) */
    seq_t c2      = 1;          /* C(n+1, 0) */
    seq_t pow2k   = 1;          /* 2^0 */

    idx_t m = n - 1;            /* upper limit for k */

    for (idx_t k = 0; k <= m; ++k) {
        sum += c1 * c2 * pow2k;

        if (k == m)
            break;

        /* Update binomial coefficients for next k */
        c1 = c1 * (m - k) / (k + 1);        /* C(n-1, k+1) */
        c2 = c2 * ((m + 2) - k) / (k + 1);  /* C(n+1, k+1) */

        pow2k <<= 1;
    }

    return sum;
}

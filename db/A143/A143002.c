/*
 * A143002  a(0) = 0, a(1) = 1, a(n+1) = 7*(2*n+1)*a(n) + n^4*a(n-1).
 * https://oeis.org/A143002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 1, recurrence: a(n+1) = 7*(2*n+1)*a(n) +
 *            n^4*a(n-1) (OEIS A143002)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev0 = 0;   /* a(0) */
    seq_t prev1 = 1;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t k = (seq_t)(i - 1);                     /* index of previous term */
        seq_t term1 = 7 * ( (seq_t)2 * k + 1 ) * prev1; /* 7*(2*k+1)*a(k) */

        /* compute k^4 without over/underflow in intermediate type */
        seq_t k2 = k * k;
        seq_t k4 = k2 * k2;
        seq_t term2 = k4 * prev0;                     /* k^4 * a(k-1) */

        seq_t cur = term1 + term2;                    /* a(k+1) */

        prev0 = prev1;
        prev1 = cur;
    }
    return prev1;   /* a(n) */
}

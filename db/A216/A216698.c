/*
 * A216698  a(n) = Sum_{k=0..n} binomial(n,k)^3 * 6^k.
 * https://oeis.org/A216698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^3 * 6^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum  = 0;          /* accumulator for the sum          */
    seq_t pow6 = 1;          /* 6^k for the current k           */
    seq_t c    = 1;          /* binomial coefficient C(n,0)     */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = C(n,k)^3 * 6^k */
        seq_t term = c * c * c;   /* c^3   */
        term = term * pow6;       /* * 6^k */
        sum  = sum + term;        /* accumulate */

        if (k == n)               /* finished all k */
            break;

        /* update for next k */
        pow6 = pow6 * (seq_t)6;                /* (6^(k+1))      */
        c    = (c * (seq_t)(n - k)) / (seq_t)(k + 1); /* C(n,k+1)   */
    }

    return sum;
}

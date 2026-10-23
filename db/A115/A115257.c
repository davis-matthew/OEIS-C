/*
 * A115257  Partial sums of binomial(2n,n)^2.
 * https://oeis.org/A115257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..n} C(2k,k)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t cb = 1;                    /* C(0,0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* add the square of the central binomial coefficient */
        sum += cb * cb;

        if (k == n) break;           /* finished */

        /* recurrence: C(2(k+1),k+1) = C(2k,k) * 4*(k+1)–2 / (k+1) */
        seq_t kk   = (seq_t)(k + 1);
        seq_t num  = kk * 4 - 2;       /* 4*(k+1) - 2 = 2*(2*(k+1)-1) */
        cb = cb * num / kk;
    }

    return sum;
}

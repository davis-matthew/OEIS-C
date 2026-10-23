/*
 * A100745  Modulo 2 binomial transform of the Jacobsthal numbers J(n).
 * https://oeis.org/A100745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..n, binomial(n,k) odd} J(k), where * Jacobsthal
 *            J(0)=0, J(1)=1, J(n)=J(n-1)+2*J(n-2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Jacobsthal numbers for k = 0 and k = 1 (k=0 is 0, k=1 is 1). */
    seq_t jPrev2 = (seq_t)0;          /* J(0) */
    seq_t jPrev1 = (seq_t)1;          /* J(1), used only if n ≥ 1 */
    seq_t sum   = (seq_t)0;

    /* k = 0 */
    if (((idx_t)0 & (n - (idx_t)0)) == 0)
        sum += jPrev2;                /* J(0) == 0, harmless             */

    /* k = 1, only if n ≥ 1 */
    if (n >= (idx_t)1) {
        if (((idx_t)1 & (n - (idx_t)1)) == 0)
            sum += jPrev1;            /* J(1) == 1                       */
    }

    /* Compute J(k) for k ≥ 2 and accumulate where binomial(n,k) is odd. */
    for (idx_t k = (idx_t)2; k <= n; ++k) {
        seq_t jCurr = jPrev1 + (seq_t)2 * jPrev2;   /* J(k) */
        if ((k & (n - k)) == 0)
            sum += jCurr;
        jPrev2 = jPrev1;   /* shift for next iteration */
        jPrev1 = jCurr;
    }

    return sum;
}

/*
 * A074354  Coefficient of q^3 in nu(n), where nu(0)=1, nu(1)=b and, for n>=2,
 *          nu(n)=b*nu(n-1)+lambda*(1+q+q^2+...+q^(n-2))*nu(n-2) with
 *          (b,lambda)=(1,2).
 * https://oeis.org/A074354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^3 in ν(n), where ν(0)=1, ν(1)=1,
 *            ν(n)=ν(n-1)+2*(1+q+...+q^(n-2))*ν(n-2) for n≥2
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 25, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n<5 the coefficient of q^3 is always 0 */
    if (n < 5)
        return (seq_t)0;

    /* Arrays holding the coefficients of q^k (k=0..3)
       for ν(n-2) (prev_prev), ν(n-1) (prev), and ν(n) (cur).   */
    seq_t prev_prev[4] = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)0 };  /* ν(0)   */
    seq_t prev[4]      = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)0 };  /* ν(1)   */
    seq_t cur[4]       = { (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0 };  /* placeholder */

    /* Iterate from n=2 up to the requested index   */
    for (idx_t i = 2; i <= n; ++i) {
        idx_t tmax = (i >= 3) ? i - 2 : 0;   /* maximum t such that t ≤ i-2          */

        for (int k = 0; k < 4; ++k) {        /* coefficient of q^k  (k≤3)          */
            seq_t sum = (seq_t)0;
            for (idx_t t = 0; t <= (idx_t)k && t <= tmax; ++t) {
                sum += prev_prev[k - (int)t];
            }
            cur[k] = prev[k] + (sum + sum);  /* prev[k] + 2*sum                    */
        }

        /* Shift the states: prev_prev ← prev, prev ← cur          */
        for (int k = 0; k < 4; ++k) {
            prev_prev[k] = prev[k];
            prev[k]      = cur[k];
        }
    }

    return cur[3];   /* coefficient of q^3 in ν(n) */
}

/*
 * A174324  a(n) = 3F0(-n,-n+1,-n+2;;-1/2) = n!*(n-1)!*2^(1-n)*
 *          1F2(-n+2;2,3;-2), where nFm(;;) are generalized hypergeometric
 *          series.
 * https://oeis.org/A174324
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   8*a(n) +4*(-3*n^2+9*n-8)*a(n-1) +6*(n-1)*(n-3)*(n-2)^2*a(n-2) *
 *            -(n-1)*(n-4)*(n-2)^2*(n-3)^2*a(n-3)=0; * Initial values: a(2)=1,
 *            a(3)=4, a(4)=31. * The recurrence is used directly; all
 *            computations are performed with * unsigned integers (seq_t). The
 *            coefficients are computed as positive * integer...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* undefined for n < 2 */

    if (n == 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)4;
    if (n == 4)
        return (seq_t)31;

    /* start with a(2), a(3), a(4) */
    seq_t a_nm3 = (seq_t)1;   /* a(n-3) for n=5 */
    seq_t a_nm2 = (seq_t)4;   /* a(n-2) for n=5 */
    seq_t a_nm1 = (seq_t)31;  /* a(n-1) for n=5 */

    for (idx_t k = 5; k <= n; ++k)
    {
        idx_t k1 = k - 1;   /* n-1 */
        idx_t k2 = k - 2;   /* n-2 */
        idx_t k3 = k - 3;   /* n-3 */
        idx_t k4 = k - 4;   /* n-4 */

        /* Bcoef = 4*(3*k^2-9*k+8)   (always positive) */
        int64_t tmp = (int64_t)3 * k * k - (int64_t)9 * k + 8;
        seq_t Bcoef = (seq_t)(4 * tmp);

        /* Cabs = 6*k1*k3*k2*k2   (always positive) */
        seq_t Cabs = (seq_t)6;
        Cabs *= (seq_t)k1;
        Cabs *= (seq_t)k3;
        Cabs *= (seq_t)k2;
        Cabs *= (seq_t)k2;

        /* Dcoef = k1*k4*k2*k2*k3*k3   (always positive) */
        seq_t Dcoef = (seq_t)k1;
        Dcoef *= (seq_t)k4;
        Dcoef *= (seq_t)k2;
        Dcoef *= (seq_t)k2;
        Dcoef *= (seq_t)k3;
        Dcoef *= (seq_t)k3;

        /* 8*a(n) = Bcoef*a(n-1) - Cabs*a(n-2) + Dcoef*a(n-3) */
        seq_t term1 = Bcoef * a_nm1;
        seq_t term2 = Dcoef * a_nm3;
        seq_t sum   = term1 + term2;          /* positive accumulator   */
        seq_t term3 = Cabs * a_nm2;
        seq_t rhs   = sum - term3;            /* sum >= term3 for all n >= 5 */

        seq_t a_n = rhs / (seq_t)8;

        /* shift for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;  /* a(n) */
}

/*
 * A203298  Second elementary symmetric function of the first n terms of
 *          (1,2,2,3,3,4,4,5,5...).
 * https://oeis.org/A203298
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( ( Σ_{k=1..n} floor((k+2)/2) )^2 - Σ_{k=1..n}
 *            floor((k+2)/2)^2 ) / 2 i.e. second elementary symmetric sum of
 *            the first n terms of (1,2,2,3,3,4,4,5,5,…)
 * Range:     u8 n <= 8, u16 n <= 36, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return 0 for indices below the offset (the behaviour is irrelevant there) */
    if (n < 2) return 0;

    /* split n into even/odd */
    idx_t half = n / 2;            /* floor(n/2)  */
    seq_t m   = (seq_t)half;       /* integer m */

    seq_t T, U;                    /* partial sums */

    if ((n & 1) == 0) {            /* n = 2m  (even) */
        /* T = Σ v(k) over first 2m terms = m^2 + 2m   */
        T = m * m + 2 * m;

        /* U = Σ v(k)^2 over first 2m terms   */
        /* U = m(m+1)(2m+1)/3 + m(m+1) + m   */
        seq_t term1 = m * (m + 1) * (2 * m + 1) / 3;
        seq_t term2 = m * (m + 1);
        U = term1 + term2 + m;
    } else {                       /* n = 2m+1 (odd) */
        /* T = Σ v(k) over first (2m+1) terms = m^2 + 3m + 1   */
        T = m * m + 3 * m + 1;

        /* U_even = Σ v(k)^2 over first 2m terms   */
        seq_t term1 = m * (m + 1) * (2 * m + 1) / 3;
        seq_t term2 = m * (m + 1);
        seq_t U_even = term1 + term2 + m;

        /* plus the last term (m+1)^2   */
        seq_t extra = (m + 1) * (m + 1);
        U = U_even + extra;
    }

    /* second elementary symmetric sum: (T^2 - U)/2   */
    return (T * T - U) / 2;
}

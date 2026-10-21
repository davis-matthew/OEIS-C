/*
 * A094187  Numerator of I(n) = Integral_{x=1..9/8} (sqrt(x^2-1)/x)^(2*n) dx.
 * https://oeis.org/A094187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) satisfies the recurrence * a(n) + (-196*n+51)*a(n-1) +
 *            2754*(n-1)*(2*n-3)*a(n-2) = 0, * with initial values a(1)=1,
 *            a(2)=35. The recurrence can be * rearranged as * a(n) =
 *            (196*n-51)*a(n-1) - 2754*(n-1)*(2*n-3)*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n<1, return 0 */
    seq_t a_nm2 = 1;            /* a(1) */
    if (n == 1)
        return a_nm2;
    seq_t a_nm1 = 35;           /* a(2) */
    if (n == 2)
        return a_nm1;

    for (idx_t k = 3; k <= n; ++k)
    {
        seq_t term1 = (196 * (seq_t)k - 51) * a_nm1;
        seq_t term2 = 2754 * (seq_t)(k - 1) * (2 * (seq_t)k - 3) * a_nm2;
        seq_t a_k   = term1 - term2;
        a_nm2 = a_nm1;
        a_nm1 = a_k;
    }
    return a_nm1;
}

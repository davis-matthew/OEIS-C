/*
 * A094856  E.g.f.: exp(4x)/(1-4x)^(1/4).
 * https://oeis.org/A094856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence a(n) + (-4*n-1)*a(n-1) +
 *            16*(n-1)*a(n-2) = 0 with initial values a(0)=1, a(1)=5
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* iterative computation as per the recurrence */
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)5;   /* a(1) */
    seq_t a_n   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* term1 = (4*i + 1) * a(i-1) */
        seq_t mult1 = (seq_t)(4*i + 1);
        seq_t term1 = mult1 * a_nm1;

        /* term2 = 16*(i-1) * a(i-2) */
        seq_t mult2 = (seq_t)(16*(i-1));
        seq_t term2 = mult2 * a_nm2;

        a_n = term1 - term2;

        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}

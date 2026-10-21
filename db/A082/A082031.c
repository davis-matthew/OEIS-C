/*
 * A082031  Expansion of e.g.f. exp(2*x)/(1-x)^3.
 * https://oeis.org/A082031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+4)*a(n-1) - 2*(n-1)*a(n-2), with a(0) = 1, a(1) = 5.
 *            This recurrence derives from the differential equation
 *            a(n)+(-n-4)a(n-1)+2(n-1)a(n-2)=0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)5;

    /* Iterate from 2 up to n */
    seq_t a_prev_2 = (seq_t)1;   /* a(0)   */
    seq_t a_prev_1 = (seq_t)5;   /* a(1)   */
    seq_t a_cur;

    for (idx_t i = 2; i <= n; ++i) {
        /* (i+4)*a_prev_1  -  2*(i-1)*a_prev_2  */
        seq_t term1 = (seq_t)(i + 4) * a_prev_1;
        seq_t term2 = (seq_t)(2 * (i - 1)) * a_prev_2;
        a_cur = term1 - term2;

        /* Shift for next iteration */
        a_prev_2 = a_prev_1;
        a_prev_1 = a_cur;
    }

    return a_cur;
}

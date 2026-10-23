/*
 * A106201  Expansion of Re(x/(1-x-2*i*x^2)), i=sqrt(-1).
 * https://oeis.org/A106201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=0, a(1)=a(2)=a(3)=1; for n≥4: a(n)=2*a(n-1) – a(n-2) –
 *            4*a(n-4)
 * Range:     i8 n <= 10, i16 n <= 19, i32 n <= 39, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1 || n == 2 || n == 3)
        return 1;

    seq_t a_n_minus_4 = 0;   /* a(0)   */
    seq_t a_n_minus_3 = 1;   /* a(1)   */
    seq_t a_n_minus_2 = 1;   /* a(2)   */
    seq_t a_n_minus_1 = 1;   /* a(3)   */
    seq_t a_n = 0;           /* current value */

    for (idx_t i = 4; i <= n; ++i) {
        a_n = 2 * a_n_minus_1 - a_n_minus_2 - 4 * a_n_minus_4;

        /* shift window for next iteration */
        a_n_minus_4 = a_n_minus_3;
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_n;
    }

    return a_n;
}

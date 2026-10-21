/*
 * A099494  A Chebyshev transform of Fibonacci(n)+(-1)^n.
 * https://oeis.org/A099494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-2)+a(n-3)-a(n-4)-a(n-6) (for n ≥ 6) with base values
 *            a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=-1, a(5)=0. The sequence is
 *            periodic with period 30, so a(n+30)=a(n)
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* static table for one periodic cycle (0 … 29) */
    static seq_t period[30];
    /* initialization flag – 0 = not initialized, 1 = initialized */
    static idx_t init_flag = 0;

    if (!init_flag) {
        period[0] = 1;
        period[1] = 0;
        period[2] = 1;
        period[3] = 1;
        period[4] = (seq_t)-1;
        period[5] = 0;
        /* compute remaining values of the first 30‑term period */
        for (idx_t i = 6; i < 30; ++i) {
            period[i] = -period[i - 2] + period[i - 3]
                         - period[i - 4] - period[i - 6];
        }
        init_flag = 1;
    }
    /* return the value corresponding to n modulo 30 */
    return period[n % 30];
}

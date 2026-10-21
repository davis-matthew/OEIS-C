/*
 * A237132  Values of x in the solutions to x^2 - 3xy + y^2 + 11 = 0, where 0
 *          < x < y.
 * https://oeis.org/A237132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) - a(n-4), initial values: a(1)=3, a(2)=4,
 *            a(3)=5, a(4)=9
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 3;
    if (n == 2) return 4;
    if (n == 3) return 5;
    if (n == 4) return 9;
    /* iterate from 5 up to n maintaining the last four terms */
    seq_t a_nm4 = 3;   /* a(1)   */
    seq_t a_nm3 = 4;   /* a(2)   */
    seq_t a_nm2 = 5;   /* a(3)   */
    seq_t a_nm1 = 9;   /* a(4)   */
    seq_t ai = 0;      /* will hold a(i) */

    for (idx_t i = 5; i <= n; ++i) {
        ai = 3 * a_nm2 - a_nm4;
        /* shift the window for the next iteration */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = ai;
    }
    return ai;
}

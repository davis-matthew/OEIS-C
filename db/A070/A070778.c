/*
 * A070778  Let M denote the 6 X 6 matrix = row by row
 *          /1,1,1,1,1,1/1,1,1,1,1,0/1,1,1,1,0,0/1,1,1,0,0,0/1,1,0,0,0,0/1,0,0,0,0,0/
 *          and A(n) the vector (x(n),y(n),z(n),t(n),u(n),v(n)) = M^n*A where
 *          A is the vector (1,1,1,1,1,1); then a(n) = u(n).
 * https://oeis.org/A070778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 2, a(2) = 11, a(3) = 41, a(4) = 176, a(5) =
 *            721, a(6) = 3003 for n > 6: a(n) = 3*a(n-1) + 6*a(n-2) -
 *            4*a(n-3) - 5*a(n-4) + a(n-5) + a(n-6)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial terms a(0)..a(6) */
    static const seq_t init[7] = {1, 2, 11, 41, 176, 721, 3003};

    if (n < 7)
        return init[n];

    /* use the last 6 terms to iterate the recurrence */
    seq_t a1 = init[1];   /* a(n-5)   */
    seq_t a2 = init[2];   /* a(n-4)   */
    seq_t a3 = init[3];   /* a(n-3)   */
    seq_t a4 = init[4];   /* a(n-2)   */
    seq_t a5 = init[5];   /* a(n-1)   */
    seq_t a6 = init[6];   /* a(n)     */
    seq_t newval;

    for (idx_t i = 7; i <= n; ++i)
    {
        newval = (seq_t)3 * a6 + (seq_t)6 * a5
                - (seq_t)4 * a4 - (seq_t)5 * a3
                + a2 + a1;

        /* shift the window forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = newval;
    }

    return a6;
}

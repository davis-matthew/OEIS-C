/*
 * A165518  Perfect squares (A000290) that can be expressed as the sum of four
 *          consecutive triangular numbers (A000217).
 * https://oeis.org/A165518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3) Initial values: a(1)=4,
 *            a(2)=100, a(3)=3364
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)           /* sequence undefined for n <= 0 – return 0 */
        return (seq_t)0;
    if (n == 1)           /* a(1) = 4 */
        return (seq_t)4;
    if (n == 2)           /* a(2) = 100 */
        return (seq_t)100;
    if (n == 3)           /* a(3) = 3364 */
        return (seq_t)3364;

    /* start the recurrence with a(1), a(2), a(3) */
    seq_t a_nm3 = (seq_t)4;        /* a(i-3) */
    seq_t a_nm2 = (seq_t)100;      /* a(i-2) */
    seq_t a_nm1 = (seq_t)3364;     /* a(i-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t tmp = (seq_t)35 * a_nm1
                  - (seq_t)35 * a_nm2
                  + a_nm3;      /* a(i) = 35*a(i-1) - 35*a(i-2) + a(i-3) */

        /* shift: a(i-2) <- a(i-1), a(i-3) <- a(i-2), a(i-1) <- a(i) */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = tmp;
    }
    return a_nm1;
}

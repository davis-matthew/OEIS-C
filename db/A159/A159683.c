/*
 * A159683  The general form of the recurrences are the a(j), b(j) and n(j)
 *          solutions of the 2 equations problem: 3*n(j) + 1 = a(j)*a(j) and
 *          5*n(j) + 1 = b(j)*b(j) with positive integer numbers.
 * https://oeis.org/A159683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 63*a(n-1) - 63*a(n-2) + a(n-3) for n>3; a(1)=0, a(2)=16,
 *            a(3)=1008
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    if (n == 2)
        return 16;

    if (n == 3)
        return 1008;

    seq_t a_nm3 = 0;     /* a(n-3)   */
    seq_t a_nm2 = 16;    /* a(n-2)   */
    seq_t a_nm1 = 1008;  /* a(n-1)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 63 * a_nm1 - 63 * a_nm2 + a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }

    return a_nm1;
}

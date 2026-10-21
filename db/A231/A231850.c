/*
 * A231850  Number of n X 3 0..2 arrays with no element having a strict
 *          majority of its horizontal and antidiagonal neighbors equal to
 *          itself plus one mod 3, with upper left element zero (rock paper
 *          and scissors drawn positions).
 * https://oeis.org/A231850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) - 41*a(n-2) + 22*a(n-3) for n>5 Initial values:
 *            a(1)=3, a(2)=34, a(3)=656, a(4)=12404, a(5)=234336
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n == 1) return 3;
    if (n == 2) return 34;
    if (n == 3) return 656;
    if (n == 4) return 12404;
    if (n == 5) return 234336;
    if (n == 0) return 0;   /* offset behaviour irrelevant */

    /* Sliding window for recurrence */
    seq_t a_nm3 = 656;      /* a(3) */
    seq_t a_nm2 = 12404;    /* a(4) */
    seq_t a_nm1 = 234336;   /* a(5) */
    seq_t a_cur = 0;

    idx_t i;
    for (i = 6; i <= n; ++i) {
        a_cur = 21 * a_nm1 - 41 * a_nm2 + 22 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }
    return a_cur;
}

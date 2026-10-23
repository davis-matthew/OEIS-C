/*
 * A135364  First column of a triangle - see Comments lines.
 * https://oeis.org/A135364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=3, a(3)=7; for n>=4: a(n) = 3*a(n-1) -
 *            2*a(n-2) + a(n-3)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 3;
    if (n == 3) return 7;

    /* For n >= 4 use the linear recurrence */
    seq_t a_nm3 = 2; /* a(1) */
    seq_t a_nm2 = 3; /* a(2) */
    seq_t a_nm1 = 7; /* a(3) */
    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t term = a_nm1;            /* start with a(n-1) */
        term += a_nm1;                 /* 2*a(n-1) */
        term += a_nm1;                 /* 3*a(n-1) */
        term += a_nm3;                 /* + a(n-3) */
        seq_t tmp = a_nm2;             /* 2*a(n-2) */
        tmp += a_nm2;
        term -= tmp;                   /* subtract 2*a(n-2) */

        /* Shift the window for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = term;
    }
    return a_nm1;
}

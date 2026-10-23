/*
 * A192802  Coefficient of x in the reduction of the polynomial (x+2)^n by
 *          x^3->x^2+x+1.
 * https://oeis.org/A192802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 15*a(n-2) + 11*a(n-3) Initial values: a(0)=0,
 *            a(1)=1, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* Keep the last three computed values. */
    seq_t a_nm3 = (seq_t)0; /* a(n-3) */
    seq_t a_nm2 = (seq_t)1; /* a(n-2) */
    seq_t a_nm1 = (seq_t)4; /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* Compute a(i) = 7*a(i-1) - 15*a(i-2) + 11*a(i-3) */
        seq_t next = 7 * a_nm1 + 11 * a_nm3;   /* 7*a(i-1) + 11*a(i-3) */
        next -= 15 * a_nm2;                   /* subtract 15*a(i-2) */

        /* Shift the window for the next iteration. */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }

    return a_nm1;
}

/*
 * A103685  Consider the morphism 1->{1,2}, 2->{1,3}, 3->{1}; a(n) is the
 *          total number of '3' after n substitutions.
 * https://oeis.org/A103685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 8*a(n-2) + 6*a(n-3) - 2*a(n-4), a(0)=0,
 *            a(1)=0, a(2)=1, a(3)=5. This counts the total number of '3's in
 *            rows 1..n for the morphism 1->{1,2}, 2->{1,3}, 3->{1}
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly. */
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 5;

    /* Variables hold successive values: 
       a(n-4), a(n-3), a(n-2), a(n-1). */
    seq_t a_nm4 = 0;   /* a(0) */
    seq_t a_nm3 = 0;   /* a(1) */
    seq_t a_nm2 = 1;   /* a(2) */
    seq_t a_nm1 = 5;   /* a(3) */
    seq_t cur = 0;

    /* Iterate up to n using the linear recurrence. */
    for (idx_t i = 4; i <= n; ++i)
    {
        cur = 5 * a_nm1 - 8 * a_nm2 + 6 * a_nm3 - 2 * a_nm4;
        /* Shift the window for the next step. */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return cur;
}

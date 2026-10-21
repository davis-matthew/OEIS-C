/*
 * A206949  Number of nonisomorphic graded posets with 0 and non-uniform Hasse
 *          graph of rank n, with no 3-element antichain.
 * https://oeis.org/A206949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 27*a(n-2) + 30*a(n-3) - 9*a(n-4) initial
 *            values: a(0)=0, a(1)=0, a(2)=0, a(3)=3, a(4)=24
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the trivial cases directly */
    if (n <= 2) return 0;
    if (n == 3) return 3;
    if (n == 4) return 24;

    /* registers holding the last four terms
       a_nm4 = a(n-4), a_nm3 = a(n-3), a_nm2 = a(n-2), a_nm1 = a(n-1) */
    seq_t a_nm4 = 0;   /* a(1) */
    seq_t a_nm3 = 0;   /* a(2) */
    seq_t a_nm2 = 3;   /* a(3) */
    seq_t a_nm1 = 24;  /* a(4) */

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next = 9*a_nm1 - 27*a_nm2 + 30*a_nm3 - 9*a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}

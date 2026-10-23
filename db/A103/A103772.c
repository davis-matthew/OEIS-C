/*
 * A103772  Larger of two sides in a (k,k,k-1)-integer-sided triangle with
 *          integer area.
 * https://oeis.org/A103772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - a(n-2) + 4 for n >= 3; a(1)=1, a(2)=17
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)  /* not part of the sequence, but safe return */
        return 0;
    if (n == 1)
        return 1;
    if (n == 2)
        return 17;

    seq_t a_nm2 = 1;   /* a(n-2)  */
    seq_t a_nm1 = 17;  /* a(n-1)  */
    seq_t a_n = 0;     /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i)
    {
        a_n = 14 * a_nm1 - a_nm2 + 4;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}

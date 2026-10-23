/*
 * A153597  a(n) = ((6 + sqrt(3))^n - (6 - sqrt(3))^n)/(2*sqrt(3)).
 * https://oeis.org/A153597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 33*a(n-2) with a(1)=1, a(2)=12 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)12;

    seq_t a_nm2 = (seq_t)1;   /* a(1)   */
    seq_t a_nm1 = (seq_t)12;  /* a(2)   */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        cur = (seq_t)12 * a_nm1 - (seq_t)33 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return a_nm1;
}

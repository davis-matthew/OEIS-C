/*
 * A174759  x-values in the solution to x^2-57*y^2=1.
 * https://oeis.org/A174759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 302*a(n-1)-a(n-2); a(1)=1, a(2)=151
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 16,
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
        return (seq_t)151;

    seq_t a_nm2 = (seq_t)1;   /* a(n-2)  -> a(1)   */
    seq_t a_nm1 = (seq_t)151; /* a(n-1)  -> a(2)   */
    seq_t a_n   = a_nm1;      /* placeholder */

    for (idx_t i = 3; i <= n; ++i) {
        a_n   = (seq_t)302 * a_nm1 - a_nm2; /* a(i)   */
        a_nm2 = a_nm1;                      /* shift      */
        a_nm1 = a_n;
    }
    return a_n;
}

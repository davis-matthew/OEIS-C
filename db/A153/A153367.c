/*
 * A153367  Number of zig-zag paths from top to bottom of a rectangle of width
 *          9 with 2n-1 rows whose color is not that of the top right corner.
 * https://oeis.org/A153367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) for n>2; a(1)=4, a(2)=14
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* below offset – arbitrary value */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)14;

    seq_t a_nm2 = (seq_t)4;     /* a(n-2) */
    seq_t a_nm1 = (seq_t)14;    /* a(n-1) */
    seq_t a_n = 0;              /* current term */

    for (idx_t i = 3; i <= n; ++i)
    {
        a_n = a_nm1;
        a_n *= (seq_t)5;
        seq_t tmp = a_nm2;
        tmp *= (seq_t)5;
        a_n -= tmp;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}

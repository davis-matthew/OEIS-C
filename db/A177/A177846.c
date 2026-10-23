/*
 * A177846  a(n) = 6*a(n-1)-8*a(n-2) for n > 2; a(0)=837, a(1)=7896,
 *          a(2)=32176.
 * https://oeis.org/A177846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1)-8*a(n-2) for n>2; a(0) = 837, a(1) = 7896, a(2)
 *            = 32176
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 26, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)837;
    if (n == 1) return (seq_t)7896;
    if (n == 2) return (seq_t)32176;

    seq_t a_nm2 = (seq_t)7896;  /* a(1) */
    seq_t a_nm1 = (seq_t)32176; /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t cur = 6 * a_nm1 - 8 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}

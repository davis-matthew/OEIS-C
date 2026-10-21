/*
 * A078368  A Chebyshev S-sequence with Diophantine property.
 * https://oeis.org/A078368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - a(n-2) with a(0)=1, a(1)=19
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)(1);
    if (n == 1)
        return (seq_t)(19);

    seq_t a_nm2 = (seq_t)(1);   /* a(n-2) */
    seq_t a_nm1 = (seq_t)(19);  /* a(n-1) */
    seq_t a_n;

    for (idx_t i = 2; i <= n; ++i) {
        a_n = (seq_t)(19) * a_nm1 - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}

/*
 * A092167  Let M = 2 X 2 matrix [ 1 2 / 5 4 ]; a(n) = (2,2) entry of M^n.
 * https://oeis.org/A092167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*6^n + 2*(-1)^n)/7 = 5*a(n-1) + 6*a(n-2); a(1)=4,
 *            a(2)=26
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)26;
    seq_t a_nm2 = (seq_t)4;   /* a(1) */
    seq_t a_nm1 = (seq_t)26;  /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t a_n = (seq_t)5 * a_nm1 + (seq_t)6 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}

/*
 * A189734  a(1)=2, a(2)=5, a(n)=2*a(n-1) + 5*a(n-2).
 * https://oeis.org/A189734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2, a(2) = 5, a(n) = 2*a(n-1) + 5*a(n-2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;

    seq_t a_nm2 = (seq_t)2;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)5;   /* a(n-1) */
    seq_t a_n   = a_nm1;      /* current term, will be updated */

    for (idx_t i = 3; i <= n; ++i) {
        a_n = 2 * a_nm1 + 5 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}

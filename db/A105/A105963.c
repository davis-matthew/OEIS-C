/*
 * A105963  Expansion of (1+4*x)/(1-x-3*x^2).
 * https://oeis.org/A105963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2), a(0) = 1, a(1) = 5
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t a_nm2 = (seq_t)1;  /* a(0) */
    seq_t a_nm1 = (seq_t)5;  /* a(1) */
    seq_t a_n;

    for (idx_t i = 2; i <= n; ++i) {
        a_n  = a_nm1 + (seq_t)3 * a_nm2;  /* a(n) = a(n-1) + 3*a(n-2) */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}

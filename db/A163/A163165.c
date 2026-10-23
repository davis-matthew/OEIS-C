/*
 * A163165  a(n) = 20*a(n-1) - 96*a(n-2) for n > 1; a(0) = 1, a(1) = 10.
 * https://oeis.org/A163165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 96*a(n-2), a(0) = 1, a(1) = 10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)10;

    seq_t a_nm2 = (seq_t)1;        /* a(0) */
    seq_t a_nm1 = (seq_t)10;       /* a(1) */
    seq_t a_n;

    for (idx_t i = 2u; i <= n; ++i) {
        a_n = a_nm1 * (seq_t)20 - a_nm2 * (seq_t)96;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}

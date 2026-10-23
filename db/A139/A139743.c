/*
 * A139743  a(n) = 11^n - 5^n.
 * https://oeis.org/A139743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - 5^n Recurrence: a(0) = 0, a(1) = 6, a(n) =
 *            16*a(n-1) - 55*a(n-2) for n >= 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 6;

    seq_t a_nm2 = 0; /* a(0) */
    seq_t a_nm1 = 6; /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t t = a_nm1 * (seq_t)16 - a_nm2 * (seq_t)55;
        a_nm2 = a_nm1;
        a_nm1 = t;
    }
    return a_nm1;
}

/*
 * A155624  a(n) = 11^n - 3^n + 1.
 * https://oeis.org/A155624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 33*a(n-2) + 20 for n ≥ 2, with a(0) = 1, a(1)
 *            = 9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return 1;
    if (n == 1) return 9;

    /* Iteratively compute terms up to n. */
    seq_t a_nm2 = 1;  /* a(n-2) */
    seq_t a_nm1 = 9;  /* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 14 * a_nm1 - 33 * a_nm2 + 20;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return cur;
}

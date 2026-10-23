/*
 * A133212  a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3), n > 3; a(0) = 1, a(1) = 4,
 *          a(2) = 12, a(3) = 32.
 * https://oeis.org/A133212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3), n > 3; a(0) = 1, a(1) =
 *            4, a(2) = 12, a(3) = 32
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)32;

    /* a(n-3), a(n-2), a(n-1) for the next iteration */
    seq_t a_nm3 = (seq_t)4; /* a(1) */
    seq_t a_nm2 = (seq_t)12;/* a(2) */
    seq_t a_nm1 = (seq_t)32;/* a(3) */

    seq_t curr = (seq_t)0; /* placeholder */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute 4*a(n-1) + 4*a(n-3) - 6*a(n-2) */
        seq_t term1 = a_nm1 * (seq_t)4 + a_nm3 * (seq_t)4;
        seq_t term2 = a_nm2 * (seq_t)6;
        curr = term1 - term2;

        /* Shift for next step */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = curr;
    }
    return curr;
}

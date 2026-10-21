/*
 * A059020  Number of 2 X n checkerboards (with at least one red square) in
 *          which the set of red squares is edge connected.
 * https://oeis.org/A059020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 4*n - 1, a(0) = 0, a(1) = 3
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;

    seq_t a_nm2 = (seq_t)0;          /* a(0) */
    seq_t a_nm1 = (seq_t)3;          /* a(1) */
    seq_t a_n;

    for (idx_t i = 2; i <= n; ++i) {
        a_n = 2 * a_nm1 + a_nm2 + ((seq_t)4 * (seq_t)i) - 1;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}

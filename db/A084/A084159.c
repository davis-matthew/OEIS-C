/*
 * A084159  Pell oblongs.
 * https://oeis.org/A084159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) + 5*a(n-2) - a(n-3) with initial values a(0) =
 *            1, a(1) = 3, a(2) = 21
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)21;

    seq_t a_nm3 = (seq_t)1;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)3;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)21;  /* a(n-1) */
    seq_t cur;
    const seq_t five = (seq_t)5;

    for (idx_t i = 3; i <= n; ++i) {
        cur = five * a_nm1 + five * a_nm2 - a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}

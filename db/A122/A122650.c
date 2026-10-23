/*
 * A122650  Fibonacci numbers starting at F(23).
 * https://oeis.org/A122650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(1) = 28657, a(2) = 46368
 * Range:     u8 none, u16 n <= 2, u32 n <= 25, u64 n <= 71, u128 n <= 164,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* undefined offset; safe default */
    if (n == 1) return (seq_t)28657;
    if (n == 2) return (seq_t)46368;

    seq_t a_nm2 = (seq_t)28657;                    /* a(n-2)  */
    seq_t a_nm1 = (seq_t)46368;                    /* a(n-1)  */
    seq_t a_n;                                     /* a(n)    */

    for (idx_t i = 3; i <= n; ++i) {
        a_n = a_nm1 + a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}

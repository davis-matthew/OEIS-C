/*
 * A203881  Number of (n+1) X 3 0..6 arrays with every 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25*a(n-1) + 138*a(n-2), a(1)=8281, a(2)=254359
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)8281;
    if (n == 2) return (seq_t)254359;

    seq_t a_nm2 = (seq_t)8281;     /* a(1)   */
    seq_t a_nm1 = (seq_t)254359;   /* a(2)   */
    seq_t a_n;

    for (idx_t i = 3; i <= n; ++i) {
        a_n = a_nm1 * (seq_t)25 + a_nm2 * (seq_t)138;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}

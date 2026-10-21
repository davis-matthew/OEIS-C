/*
 * A074624  a(n) = 8^n + 9^n.
 * https://oeis.org/A074624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8^n + 9^n, a(n) = 17*a(n-1) - 72*a(n-2) with a(0)=2,
 *            a(1)=17
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)2;
    if (n == (idx_t)1) return (seq_t)17;

    seq_t a_n_minus_2 = (seq_t)2;   /* a(0) */
    seq_t a_n_minus_1 = (seq_t)17;  /* a(1) */
    seq_t a_n;

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        a_n = (seq_t)17 * a_n_minus_1 - (seq_t)72 * a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_n;
    }
    return a_n;
}

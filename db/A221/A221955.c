/*
 * A221955  a(n) = 6^(n-1) * n! * Catalan(n-1).
 * https://oeis.org/A221955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*(2*n-3)*a(n-1), a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t a_val = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t factor = (seq_t)12 * (((seq_t)2 * (seq_t)i) - (seq_t)3);
        a_val *= factor;
    }
    return a_val;
}

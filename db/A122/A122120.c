/*
 * A122120  a(n) = 4*a(n-1) + 9*a(n-2), for n>1, with a(0)=1, a(1)=3.
 * https://oeis.org/A122120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 3, a(n) = 4*a(n-1) + 9*a(n-2) for n>1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)3;
    seq_t a0 = (seq_t)1;  /* a(n-2) */
    seq_t a1 = (seq_t)3;  /* a(n-1) */
    seq_t an;
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        an = (seq_t)4 * a1 + (seq_t)9 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}

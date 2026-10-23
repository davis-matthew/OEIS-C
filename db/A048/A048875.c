/*
 * A048875  Generalized Pellian with second term of 6.
 * https://oeis.org/A048875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2), with a(0)=1, a(1)=6
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t a0 = (seq_t)1; /* a(n-2) */
    seq_t a1 = (seq_t)6; /* a(n-1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = (seq_t)4 * a1 + a0; /* a(n) = 4*a(n-1) + a(n-2) */
        a0 = a1;
        a1 = next;
    }
    return a1;
}

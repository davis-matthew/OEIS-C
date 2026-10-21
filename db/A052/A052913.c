/*
 * A052913  a(n+2) = 5*a(n+1) - 2*a(n), with a(0) = 1, a(1) = 4.
 * https://oeis.org/A052913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 4, a(n) = 5*a(n-1) - 2*a(n-2) for n >= 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t a0 = (seq_t)1;   /* a(n-2) */
    seq_t a1 = (seq_t)4;   /* a(n-1) */
    seq_t an = a1;         /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)5 * a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = an;
    }
    return an;
}

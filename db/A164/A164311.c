/*
 * A164311  a(n) = 12*a(n-1) - 33*a(n-2) for n > 1; a(0) = 4, a(1) = 27.
 * https://oeis.org/A164311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 33*a(n-2), a(0)=4, a(1)=27
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a0 = 4;
    if (n == 0) return a0;
    seq_t a1 = 27;
    if (n == 1) return a1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t a2 = 12 * a1 - 33 * a0;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}

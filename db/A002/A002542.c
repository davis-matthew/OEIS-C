/*
 * A002542  Number of two-valued complete Post functions of n variables.
 * https://oeis.org/A002542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2^n-2) - 2^(2^(n-1)-1) = m*(m-1) with m =
 *            2^(2^(n-1)-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = 1;
    seq_t p = 2;
    idx_t i;
    for (i = 1; i < n; i++) {
        m *= p;
        p *= p;
        if (m == 0) break;
    }
    return m * (m - 1);
}

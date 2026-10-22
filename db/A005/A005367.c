/*
 * A005367  a(n) = 2*(2^n + 1)*(2^(n+1) - 1).
 * https://oeis.org/A005367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n+1) + 2^(n+1) - 2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p = 1;
    idx_t i;
    for (i = 0; i <= n; i++)
        p *= 2;
    return (seq_t)(p * p + p - 2);
}

/*
 * A002446  a(n) = 2^(2*n+1) - 2.
 * https://oeis.org/A002446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n+1) - 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p = 2;
    idx_t i;
    for (i = 0; i < n; i++)
        p *= 4;
    return p - 2;
}

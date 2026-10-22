/*
 * A001240  Expansion of 1/((1-2x)(1-3x)(1-6x)).
 * https://oeis.org/A001240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^(n-1) + 3^n*(2^(n-1) - 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p2 = 1;
    seq_t p3 = 3;
    idx_t i;
    for (i = 1; i < n; i++) {
        p2 *= 2;
        p3 *= 3;
    }
    return p3 * (p2 - 1) + p2;
}

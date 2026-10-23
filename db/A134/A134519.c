/*
 * A134519  Numbers remaining when the natural numbers (A000027) are arranged
 *          into a triangle and only the beginning and end terms of each row
 *          are retained.
 * https://oeis.org/A134519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000217(floor((n+1)/2)) + (1 + (-1)^n)/2
 * Range:     u8 n <= 44, u16 n <= 722, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* m = floor((n+1)/2) */
    idx_t m = (n + 1) >> 1;
    seq_t mm = (seq_t)m;
    seq_t tri = mm * (mm + 1) / 2;
    seq_t add = (n & 1) ? 0 : 1;
    return tri + add;
}

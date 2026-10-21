/*
 * A206546  Period 8: repeat [1, 7, 11, 13, 13, 11, 7, 1].
 * https://oeis.org/A206546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -((n-1) mod 8)^2 + 7*((n-1) mod 8) + 1
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at n = 1 */
    idx_t k = (n - 1) & 7;         /* (n-1) mod 8, quicker with bit mask */
    seq_t kk = (seq_t)k;
    return 7 * kk + 1 - kk * kk;   /* equivalent to -kk^2 + 7*kk + 1 */
}

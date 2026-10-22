/*
 * A005943  Factor complexity (number of subwords of length n) of the
 *          Golay-Rudin-Shapiro binary word A020987.
 * https://oeis.org/A005943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(0) = 1; a(1..7) = 2,4,8,16,24,36,46; a(n) = 8*n - 8 for n >= 8
 * Range:     u8 n <= 32, u16 n <= 8192, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 4;
    if (n == 3) return 8;
    if (n == 4) return 16;
    if (n == 5) return 24;
    if (n == 6) return 36;
    if (n == 7) return 46;
    return 8 * (seq_t)n - 8;
}

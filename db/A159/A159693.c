/*
 * A159693  Partial sums of A000463.
 * https://oeis.org/A159693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 3*n^2 + 8*n + r(n)) / 24 * where r(n) = 3*n + 9 if
 *            n is odd, and r(n) = 3*n^2 if n is even
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 1782, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n < 1, return 0 for safety */
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t r = ((n & 1ULL) ? (3 * N + 9) : (3 * n2));
    seq_t num = n3 + 3 * n2 + 8 * N + r;
    return num / 24;
}

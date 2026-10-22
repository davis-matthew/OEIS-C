/*
 * A004488  Tersum n + n.
 * https://oeis.org/A004488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(3*n) = 3*a(n), a(3*n+1) = 3*a(n)+2, a(3*n+2) = 3*a(n)+1, a(0)
 *            = 0
 * Range:     u8 n <= 242, u16 n <= 9287, u32 n <= 9287, u64 n <= 9287,
 *            u128 n <= 9287, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t q = a(n / 3);
    idx_t r = n % 3;
    if (r == 1) return 3 * q + 2;
    if (r == 2) return 3 * q + 1;
    return 3 * q;
}

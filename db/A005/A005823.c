/*
 * A005823  Numbers whose ternary expansion contains no 1's.
 * https://oeis.org/A005823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, look, base
 * Formula:   a(2n) = 3*a(n)+2, a(2n+1) = 3*a(n+1), a(1) = 0
 * Range:     u8 n <= 32, u16 n <= 1024, u32 n <= 8624, u64 n <= 8624,
 *            u128 n <= 8624, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n % 2 == 0) {
        return 3 * a(n / 2) + 2;
    } else {
        return 3 * a((n + 1) / 2);
    }
}

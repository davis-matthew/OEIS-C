/*
 * A002487  Stern's diatomic series (or Stern-Brocot sequence): a(0) = 0, a(1)
 *          = 1; for n > 0: a(2*n) = a(n), a(2*n+1) = a(n) + a(n+1).
 * https://oeis.org/A002487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, core, look, changed
 * Formula:   a(0) = 0, a(1) = 1; for n > 0: a(2*n) = a(n), a(2*n+1) = a(n) +
 *            a(n+1)
 * Range:     u8 n <= 4690, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n % 2 == 0) {
        return a(n / 2);
    } else {
        return a(n / 2) + a(n / 2 + 1);
    }
}

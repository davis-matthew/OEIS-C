/*
 * A008254  Coordination sequence T1 for feldspar.
 * https://oeis.org/A008254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3*m) = 20*m^2, a(3*m+1) = 20*m^2 + 14*m + 4, a(3*m+2) = 20*m^2
 *            + 26*m + 12 (m > 0), a(0) = 1, a(1) = 4, a(2) = 10
 * Range:     u8 n <= 10, u16 n <= 171, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 4;
    if (n == 2) return 10;
    idx_t m = n / 3;
    seq_t mm = (seq_t)m;
    seq_t base = (seq_t)20 * mm * mm;
    idx_t r = n % 3;
    if (r == 1) return base + (seq_t)14 * mm + 4;
    if (r == 2) return base + (seq_t)26 * mm + 12;
    return base; /* r == 0 */
}

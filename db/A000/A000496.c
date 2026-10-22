/*
 * A000496  Restricted permutations.
 * https://oeis.org/A000496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - 8 for n >= 8, with a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=6, a(4)=24, a(5)=44, a(6)=80, a(7)=144
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 35, u64 n <= 71, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const int init[8] = {1, 1, 2, 6, 24, 44, 80, 144};
    if (n < 8) return (seq_t)init[n];
    seq_t x = (seq_t)44, y = (seq_t)80, z = (seq_t)144;
    for (idx_t i = 8; i <= n; i++) {
        seq_t w = x + y + z - 8;
        x = y;
        y = z;
        z = w;
    }
    return z;
}

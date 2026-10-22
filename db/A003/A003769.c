/*
 * A003769  Number of perfect matchings (or domino tilings) in K_4 X P_n.
 * https://oeis.org/A003769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2) - a(n-3), a(1) = 3, a(2) = 16, a(3) =
 *            75
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)3, y = (seq_t)16, z = (seq_t)75, t;
    idx_t i;
    if (n == 1) return x;
    if (n == 2) return y;
    for (i = 4; i <= n; i++) {
        t = (seq_t)4 * z + (seq_t)4 * y - x;
        x = y;
        y = z;
        z = t;
    }
    return z;
}

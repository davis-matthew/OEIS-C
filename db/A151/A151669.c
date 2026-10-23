/*
 * A151669  G.f.: Product_{k>=0} (1 + 2*x^(4^k)).
 * https://oeis.org/A151669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{# of 1‑digits in the base‑4 representation of n} if
 *            all base‑4 digits are 0 or 1, otherwise 0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n;
    int ones = 0;
    while (m > 0) {
        idx_t d = m % 4;
        if (d > 1) return 0;
        if (d == 1) ++ones;
        m /= 4;
    }
    seq_t res = 1;
    for (int i = 0; i < ones; ++i) res *= 2;
    return res;
}

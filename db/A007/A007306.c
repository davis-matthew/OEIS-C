/*
 * A007306  Denominators of Farey tree fractions (i.e., the Stern-Brocot
 *          subtree in the range [0,1]).
 * https://oeis.org/A007306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, tabf, nice
 * Formula:   a(0) = 1; for n > 0, a(n) = A002487(2*n-1), where A002487 is
 *            Stern's diatomic sequence: s(0) = 0, s(1) = 1, s(2*m) = s(m),
 *            s(2*m+1) = s(m) + s(m+1)
 * Range:     u8 n <= 2345, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9461 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t stern(idx_t m) {
    seq_t x = 1, y = 0;   /* invariant: s(original) = x*s(m) + y*s(m+1) */
    while (m > 0) {
        if (m & 1) y += x;
        else x += y;
        m >>= 1;
    }
    return y;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    return stern(2 * n - 1);
}

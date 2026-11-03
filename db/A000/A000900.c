/*
 * A000900  Number of solutions to the rook problem on an n X n board having a
 *          certain symmetry group (see Robinson for details).
 * https://oeis.org/A000900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (A000085(n) - A000898(floor(n/2)))/2, computed as a(n) =
 *            u(n) - v(floor(n/2)) with u(n) = A000085(n)/2: u(2) = 1, u(3) =
 *            2, u(n) = u(n-1) + (n-1)*u(n-2); and v(m) = A000898(m)/2: v(1) =
 *            1, v(2) = 3, v(m) = 2*v(m-1) + 2*(m-1)*v(m-2); a(0) = a(1) = 0
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 32, u128 n <= 54,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 197 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t half_a000085(idx_t n) {
    /* valid for n >= 2 */
    seq_t p2 = 1, p1 = 2; /* u(2), u(3) */
    if (n == 2) return p2;
    for (idx_t i = 4; i <= n; i++) {
        seq_t c = p1 + (seq_t)(i - 1) * p2;
        p2 = p1;
        p1 = c;
    }
    return p1;
}

static seq_t half_a000898(idx_t m) {
    /* valid for m >= 1 */
    seq_t p2 = 1, p1 = 3; /* v(1), v(2) */
    if (m == 1) return p2;
    for (idx_t i = 3; i <= m; i++) {
        seq_t c = 2 * p1 + 2 * (seq_t)(i - 1) * p2;
        p2 = p1;
        p1 = c;
    }
    return p1;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    return half_a000085(n) - half_a000898(n / 2);
}

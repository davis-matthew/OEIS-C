/*
 * A002707  Number of ternary trees with n nodes.
 * https://oeis.org/A002707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = T(n) - T(n-1), where T(n) = 1 + binomial(T(n-1)+2, 3),
 *            T(0) = 1, a(0) = 1 (T(n) counts ternary commutative trees of
 *            height <= n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 7 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t mc3(seq_t t) {
    /* t*(t+1)*(t+2)/6, with the divisions done before multiplying */
    seq_t x = t, y = t + 1, z = t + 2;
    if (x % 3 == 0) x /= 3;
    else if (y % 3 == 0) y /= 3;
    else z /= 3;
    if (x % 2 == 0) x /= 2;
    else y /= 2;
    return x * y * z;
}

seq_t a(idx_t n) {
    seq_t prev = 1; /* T(0) */
    if (n == 0) return 1;
    seq_t cur = prev;
    for (idx_t i = 1; i <= n; i++) {
        prev = cur;
        cur = 1 + mc3(prev);
    }
    return cur - prev;
}

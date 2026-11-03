/*
 * A000899  Number of solutions to the rook problem on an n X n board having a
 *          certain symmetry group (see Robinson for details).
 * https://oeis.org/A000899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (A000142(n) - 2*A000085(n) - A037223(n) +
 *            2*A000898(floor(n/2)))/8, with A000085(n) = A000085(n-1) +
 *            (n-1)*A000085(n-2), A000085(0) = A000085(1) = 1; A037223(n) =
 *            2^m * m!, m = floor(n/2); A000898(m) = 2*(A000898(m-1) +
 *            (m-1)*A000898(m-2)), A000898(0) = 1, A000898(1) = 2. Computed as
 *            n!/8...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 171 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t a037223_val(idx_t n) {
    idx_t m = n / 2;
    seq_t r = 1;
    for (idx_t i = 1; i <= m; i++) r *= (seq_t)(2 * i);
    return r;
}

static seq_t a898_val(idx_t m) {
    seq_t p2 = 1, p1 = 2;
    if (m == 0) return 1;
    if (m == 1) return 2;
    for (idx_t i = 2; i <= m; i++) {
        seq_t c = 2 * (p1 + (seq_t)(i - 1) * p2);
        p2 = p1;
        p1 = c;
    }
    return p1;
}

static seq_t inv_count(idx_t n) {
    seq_t p2 = 1, p1 = 1;
    if (n < 2) return 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t c = p1 + (seq_t)(i - 1) * p2;
        p2 = p1;
        p1 = c;
    }
    return p1;
}

seq_t a(idx_t n) {
    seq_t S = 2 * inv_count(n) + a037223_val(n) - 2 * a898_val(n / 2);
    if (n < 4) {
        seq_t f = 1;
        for (idx_t i = 2; i <= n; i++) f *= (seq_t)i;
        return (f - S) / 8;
    }
    /* n!/8 = 3 * 5 * 6 * ... * n */
    seq_t f8 = 3;
    for (idx_t i = 5; i <= n; i++) f8 *= (seq_t)i;
    return f8 - S / 8;
}

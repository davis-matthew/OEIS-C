/*
 * A042408  Numerators of continued fraction convergents to sqrt(732).
 * https://oeis.org/A042408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 182 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A042408: numerators of the continued fraction convergents to sqrt(732):
 * 27, 487, 26325, 474337 and a(n) = 974 a(n-2) - a(n-4), computed directly
 * (ring operations, exact modulo the width; the original's first call wrote
 * past its 4-element cache). */

seq_t a(idx_t n)
{
    static const seq_t first[4] = {27, 487, 26325, 474337};
    if (n < 4) return first[n];
    seq_t w = first[0], x = first[1], y = first[2], z = first[3];   /* a(i-4) .. a(i-1) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t t = (seq_t)974 * y - w;
        w = x; x = y; y = z; z = t;
    }
    return z;
}

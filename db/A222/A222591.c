/*
 * A222591  Numerators of (n*(n - 3)/6) + 1, arising as the maximum possible
 *          number of triple lines for an n-element set.
 * https://oeis.org/A222591
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of (n*(n-3)/6)+1. The numerator is
 *            (n*(n-3)+6)/gcd(n*(n-3)+6, 6)
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n) {
    idx_t v = n * (n - 3) + (idx_t)6;
    idx_t d = gcd_idx(v, (idx_t)6);
    return (seq_t)(v / d);
}

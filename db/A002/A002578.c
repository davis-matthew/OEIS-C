/*
 * A002578  Number of integral points in a certain sequence of open
 *          quadrilaterals.
 * https://oeis.org/A002578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: x^3*(x^5+x^4+x^2+x+1)/((1-x^6)*(1-x)^2)
 * Range:     u8 n <= 26, u16 n <= 397, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 962 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t f(seq_t m) {
    seq_t k = m / 6;
    seq_t r = m % 6;
    return (k + 1) * (3 * k + r + 1);
}

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t result = 0;
    if (n >= 3) result += f(m - 3);
    if (n >= 4) result += f(m - 4);
    if (n >= 5) result += f(m - 5);
    if (n >= 7) result += f(m - 7);
    if (n >= 8) result += f(m - 8);
    return result;
}

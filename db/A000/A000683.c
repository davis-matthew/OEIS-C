/*
 * A000683  Number of colorings of labeled graphs on n nodes using exactly 2
 *          colors, divided by 4.
 * https://oeis.org/A000683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (1/4) * Sum_{i=1..n-1} binomial(n,i) * 2^(i*(n-i))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 14, u128 n <= 21,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int a683_width(void) {
    seq_t x = 1;
    int w = 0;
    while (x != 0 && w <= 200) {
        x = (seq_t)(x << 1);
        w++;
    }
    return (w > 200) ? 0 : w; /* 0 means arbitrary precision */
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    int w = a683_width();
    seq_t result = 0;
    seq_t binom = 1; /* binomial(n, i) */
    for (idx_t i = 1; i < n; i++) {
        binom = binom * (seq_t)(n - i + 1) / (seq_t)i;
        idx_t e = i * (n - i) - 2; /* >= 0 for n >= 3 */
        if (w != 0 && e >= (idx_t)w) continue;
        result += (seq_t)(binom << e);
    }
    return result;
}

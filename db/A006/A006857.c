/*
 * A006857  a(n) = binomial(n+5,5) * binomial(n+5,4)/(n+5).
 * https://oeis.org/A006857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+4,4)*C(n+5,4)/5
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 40, u64 n <= 689, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 990 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t a006857_binom4(idx_t m) {
    seq_t r = 1;
    idx_t i;
    for (i = 0; i < 4; i++) {
        r = r * (seq_t)(m - i) / (seq_t)(i + 1);
    }
    return r;
}

seq_t a(idx_t n) {
    seq_t x = a006857_binom4(n + 4);
    seq_t y = a006857_binom4(n + 5);
    if (x % 5 == 0)
        x /= 5;
    else
        y /= 5;
    return x * y;
}

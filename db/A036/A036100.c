/*
 * A036100  Centered cube numbers: (n+1)^22 + n^22.
 * https://oeis.org/A036100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^22 + n^22
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 6, u128 n <= 54,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 8874 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow22(seq_t x) {
    seq_t result = 1;
    for (int i = 0; i < 22; ++i)
        result *= x;
    return result;
}

seq_t a(idx_t n) {
    seq_t n1 = (seq_t)n;
    seq_t term1 = pow22(n1 + 1);
    seq_t term0 = pow22(n1);
    return term1 + term0;
}

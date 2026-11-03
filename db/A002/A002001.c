/*
 * A002001  a(n) = 3*4^(n-1), n>0; a(0)=1.
 * https://oeis.org/A002001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*4^(n-1) for n>0; a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 283 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow4(idx_t e) {
    if (e == 0) return 1;
    seq_t result = 1;
    seq_t base = 4;
    while (e > 0) {
        if (e & 1) result = result * base;
        base = base * base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    return (seq_t)3 * pow4(n - 1);
}

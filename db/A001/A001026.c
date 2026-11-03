/*
 * A001026  Powers of 17: a(n) = 17^n.
 * https://oeis.org/A001026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1), a(0) = 1 (i.e. a(n) = 17^n), via binary
 *            exponentiation
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 95 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t a001026_pow17(idx_t e) {
    seq_t result = 1;
    seq_t base = 17;
    while (e > 0) {
        if (e & 1) result *= base;
        e >>= 1;
        if (e > 0) base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    return a001026_pow17(n);
}

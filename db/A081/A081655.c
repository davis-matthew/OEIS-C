/*
 * A081655  a(n) = 2*5^n-1.
 * https://oeis.org/A081655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*5^n - 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 977 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_of_5(idx_t exp) {
    seq_t result = 1;
    seq_t base = 5;
    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t p = pow_of_5(n);
    return p * 2 - 1;
}

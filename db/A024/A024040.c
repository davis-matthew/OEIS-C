/*
 * A024040  a(n) = 4^n - n^4.
 * https://oeis.org/A024040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4^n - n^4
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 15, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 198 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow4n = pow4(n);
    seq_t nn = (seq_t)n;
    seq_t n4 = nn * nn * nn * nn;
    return pow4n - n4;
}

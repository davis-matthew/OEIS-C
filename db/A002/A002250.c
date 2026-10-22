/*
 * A002250  a(n) = 4^n - 2*3^n.
 * https://oeis.org/A002250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4^n - 2*3^n
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 15, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t ipow(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t pow4 = ipow(4, n);
    seq_t pow3 = ipow(3, n);
    return pow4 - 2 * pow3;
}

/*
 * A002488  a(n) = n^(n^n).
 * https://oeis.org/A002488
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign, nice
 * Formula:   a(n) = n^(n^n)
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 2, i64 n <= 3, i128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power_seq(seq_t base, seq_t exp) {
    if (exp == 0) return 1;
    if (base == 0) return 0;
    if (base == 1) return 1;
    if (base == -1) return (exp & 1) ? -1 : 1;
    
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
    if (n == -1) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    seq_t base = (seq_t)n;
    seq_t exp_inner = power_seq(base, base);
    return power_seq(base, exp_inner);
}

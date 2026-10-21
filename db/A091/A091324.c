/*
 * A091324  a(n) = (2^n)^(2^n)-(2^n-1)^(2^n).
 * https://oeis.org/A091324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n)^(2^n) - (2^n-1)^(2^n)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Compute pow for big integers: base = 2^n, exp = 2^n  */
    seq_t base = (seq_t)1 << n;                 /* 2^n */
    idx_t exp = (idx_t)1 << n;                  /* 2^n as exponent, works for n < width of idx_t */
    seq_t term1 = pow_seq_t(base, exp);          /* (2^n)^(2^n) */
    seq_t base2 = base - (seq_t)1;              /* 2^n - 1 */
    seq_t term2 = pow_seq_t(base2, exp);         /* (2^n - 1)^(2^n) */
    return term1 - term2;
}

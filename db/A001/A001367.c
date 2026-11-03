/*
 * A001367  Solution to f(2) = 1, f(n) = sqrt(n) f(sqrt(n)) + n at values n =
 *          2^2^i.
 * https://oeis.org/A001367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2^n - 1) * (2*n + 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 12 terms through n = 11 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t a000802_pow2pow2(idx_t e) {
    /* 2^(2^e), by squaring 2 e times */
    seq_t r = 2;
    idx_t i;
    for (i = 0; i < e; i++) r = r * r;
    return r;
}

seq_t a(idx_t n) {
    seq_t pow2pow2_n = a000802_pow2pow2(n);
    seq_t factor = (seq_t)2 * (seq_t)n + (seq_t)1;
    return pow2pow2_n / 2 * factor;
}

/*
 * A130624  Binomial transform of A101000.
 * https://oeis.org/A130624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n + a(n-1) - a(n-2), with a(0) = 0, a(1) = 1. This
 *            follows from the given recurrence a(n) = 3*a(n-1) - 3*a(n-2) +
 *            2*a(n-3)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    seq_t a_nm2 = 0;  /* a(0) */
    seq_t a_nm1 = 1;  /* a(1) */
    seq_t a_n = 0;
    seq_t pow_val = 2;   /* 2^1 */
    for (idx_t i = 2; i <= n; ++i) {
        pow_val = pow_val + pow_val;          /* pow_val = 2^i */
        a_n = pow_val + a_nm1 - a_nm2;        /* a(i) = 2^i + a(i-1) - a(i-2) */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}

/*
 * A005972  Partial sums of fourth powers of Lucas numbers.
 * https://oeis.org/A005972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000045(4*n+2) + 4*(-1)^n*(A000045(n)^2 + A000045(n+1)^2)
 *            + 6*n-5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t top = 4 * n + 2;
    seq_t f0 = 0, f1 = 1;   /* f0 = F(i), f1 = F(i+1) */
    seq_t fn = 0, fn1 = 0;
    idx_t i;
    for (i = 0; i < top; i++) {
        if (i == n) {
            fn = f0;
            fn1 = f1;
        }
        seq_t t = f0 + f1;
        f0 = f1;
        f1 = t;
    }
    /* now f0 = F(4n+2) */
    seq_t term2 = fn * fn + fn1 * fn1;
    seq_t result = f0 + 6 * (seq_t)n;
    if (n % 2 == 0) {
        result += 4 * term2;
    } else {
        result -= 4 * term2;
    }
    result -= 5;
    return result;
}

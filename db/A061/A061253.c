/*
 * A061253  Let G_n be the elementary Abelian group G_n = (C_3)^n; a(n) is the
 *          number of times the number 1 appears in the character table of
 *          G_n.
 * https://oeis.org/A061253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-1) * (3^n + 2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow3(idx_t exp) {
    seq_t result = (seq_t)1;
    seq_t base = (seq_t)3;
    while (exp) {
        if (exp & (idx_t)1) {
            result *= base;
        }
        exp >>= (idx_t)1;
        if (exp) {
            base *= base;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t pow3_n = pow3(n);          /* 3^n */
    seq_t pow3_n_1 = pow3(n - (idx_t)1); /* 3^(n-1) */
    return pow3_n_1 * (pow3_n + (seq_t)2);
}

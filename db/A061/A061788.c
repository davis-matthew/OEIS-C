/*
 * A061788  a(n) = Sum_{k=1..n} (2k)^(2k).
 * https://oeis.org/A061788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (2k)^(2k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & (idx_t)1) {
            result *= base;
        }
        exp >>= 1;
        if (exp) {
            base *= base;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    /* a(0) is undefined; for safety return 0 when n==0 */
    if (n == 0) return (seq_t)0;

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t base = (seq_t)(2 * k);     /* 2k */
        seq_t term = pow_seq_t(base, (idx_t)(2 * k)); /* (2k)^(2k) */
        sum += term;
    }
    return sum;
}

/*
 * A061104  Smallest number whose sum of digits is n^2.
 * https://oeis.org/A061104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ((n^2 mod 9)+1)*10^floor(n^2/9)-1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow10_exp(idx_t e) {
    seq_t result = 1;
    seq_t base   = 10;
    while (e) {
        if (e & 1)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* compute n^2 in the big integer type */
    seq_t s = (seq_t)n * (seq_t)n;
    /* n^2 mod 9 */
    seq_t mod9 = s % 9;
    /* floor(n^2 / 9) – fits in idx_t for all practical callers */
    idx_t exp = (idx_t)(s / 9);
    /* 10^floor(n^2/9) */
    seq_t pow10 = pow10_exp(exp);
    /* (mod9 + 1) * pow10 - 1 */
    seq_t multiplier = mod9 + 1;
    seq_t res = multiplier * pow10 - 1;
    return res;
}

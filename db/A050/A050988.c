/*
 * A050988  6-idempotent numbers.
 * https://oeis.org/A050988
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,6) * 6^(n-6) for n >= 6
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 6) {
        /* Undefined for indices below the offset; return 0 for safety */
        return 0;
    }

    /* Compute binomial coefficient binomial(n,6) = n*(n-1)*...*(n-5)/720 */
    seq_t binom = 1;
    for (idx_t i = 1; i <= 6; ++i) {
        /* Multiply by (n-5+i) then divide by i (exact division) */
        binom *= (seq_t)(n - 6 + i);
        binom /= (idx_t)i;
    }

    /* Compute 6^(n-6) by exponentiation by squaring */
    seq_t power = 1;
    seq_t base  = 6;
    idx_t exp   = n - 6;
    while (exp > 0) {
        if (exp & 1) {
            power *= base;
        }
        base *= base;
        exp >>= 1;
    }

    return binom * power;
}

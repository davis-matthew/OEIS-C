/*
 * A093379  Expansion of x(1-2x-2x^2)/((1+x)(1-2x)(1-3x)).
 * https://oeis.org/A093379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3^n - (-1)^n + 4 * 2^n) / 12 (integer division)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Compute 3^n and 2^n */
    seq_t pow3 = pow_seq((seq_t)3, n);
    seq_t pow2 = pow_seq((seq_t)2, n);

    /* numerator = 3^n + 4*2^n - (-1)^n */
    seq_t num = pow3 + ((((seq_t)4) * pow2));
    if ((n & (idx_t)1) == (idx_t)0) {
        /* even n: subtract 1 */
        num -= (seq_t)1;
    } else {
        /* odd n: add 1 (subtract -1) */
        num += (seq_t)1;
    }

    /* divide by 12 */
    return num / (seq_t)12;
}

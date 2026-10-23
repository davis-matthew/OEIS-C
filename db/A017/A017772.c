/*
 * A017772  Binomial coefficients C(56,n).
 * https://oeis.org/A017772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(56, n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* If n is outside the range 0..56, the coefficient is zero. */
    if (n > 56) {
        return 0;
    }

    /* Use symmetry to reduce the number of multiplications. */
    idx_t k = n;
    if (k > 56 - k) {
        k = 56 - k;
    }

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply by the numerator term. */
        result = result * (56 - k + i);
        /* Divide by the denominator term – the division is exact. */
        result = result / i;
    }
    return result;
}

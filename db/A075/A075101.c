/*
 * A075101  Numerator of 2^n/n.
 * https://oeis.org/A075101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^(n - v2(n)) (v2(n) = exponent of 2 in n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute the 2-adic valuation of n. */
    idx_t v2 = 0;
    idx_t t = n;
    while ((t & 1u) == 0u) {
        ++v2;
        t >>= 1;
    }

    /* Exponent for the final power of two. */
    idx_t exp = n - v2;

    /* Compute 2^exp by exponentiation by squaring. */
    seq_t result = (seq_t)1;
    seq_t base = (seq_t)2;
    while (exp > 0) {
        if (exp & 1u) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

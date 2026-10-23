/*
 * A183896  Imaginary part of a (-4,-4) Gaussian integer Somos-4 sequence.
 * https://oeis.org/A183896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 0 for n ≡ 0 or 3 (mod 4) a(n) = -2^e for n ≡ 1 (mod 4)
 *            with e = 4k² + 3k a(n) = -2^e for n ≡ 2 (mod 4) with e = 4k² +
 *            5k + 1 where k = ⌊n/4⌋. This matches the OEIS expression a(n) =
 *            mod(binomial(n+1,2),2) * A183895(n). The sequence grows as
 *            powers of two; the above closed form yields the s...
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 16, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Fast remainder modulo 4 */
    const idx_t r = n & 3U;          /* n % 4, using bitwise AND */

    /* Zero for indices 0 or 3 modulo 4 */
    if (r == 0U || r == 3U) {
        return 0;
    }

    /* Common divisor k = floor(n/4) */
    const idx_t k = n / 4U;
    idx_t exp;

    if (r == 1U) {
        /* n ≡ 1 (mod 4): exponent e = 4k² + 3k */
        exp = 4 * k * k + 3 * k;
    } else {                    /* r == 2U, n ≡ 2 (mod 4) */
        /* exponent e = 4k² + 5k + 1 */
        exp = 4 * k * k + 5 * k + 1U;
    }

    /* Compute 2^exp and negate */
    seq_t val = ((seq_t)1 << exp);
    return -val;
}

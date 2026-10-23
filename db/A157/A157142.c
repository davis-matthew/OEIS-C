/*
 * A157142  Signed denominators of Leibniz series for Pi/4.
 * https://oeis.org/A157142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 63, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (-1)^n * (2n + 1)
 * This implements the closed‑form for OEIS A157142.
 */

seq_t a(idx_t n) {
    /* Convert index to seq_t first */
    seq_t x = (seq_t)n;

    /* Compute absolute value 2n + 1 */
    seq_t val = (seq_t)(2 * x + 1);

    /* Determine the sign: (-1)^n */
    seq_t sign = (n & 1) ? (seq_t)(-1) : (seq_t)(1);

    return sign * val;
}

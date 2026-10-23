/*
 * A100219  Expansion of (1-2*x)/((1-x)*(1-x+x^2)).
 * https://oeis.org/A100219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = [1, 0, -2, -3, -2, 0] repeated with period 6
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is periodic of length 6.  Use a small lookup table. */
    static const seq_t pattern[6] = {
        (seq_t)1,
        (seq_t)0,
        (seq_t)(-2),
        (seq_t)(-3),
        (seq_t)(-2),
        (seq_t)0
    };
    return pattern[n % 6];
}

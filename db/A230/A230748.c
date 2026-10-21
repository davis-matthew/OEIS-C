/*
 * A230748  Duplicate of A078607.
 * https://oeis.org/A230748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = round(n / ln(2)) (n ≥ 2), and a(1) = 2. Here rounding is
 *            performed by adding half of the denominator before truncating.
 *            The constant 1/ln(2) ≈ 1.4426950408889634 is represented as the
 *            rational 1442695/1000000, which is accurate enough for all n ≤
 *            10 000, giving the correct integer result
 * Range:     i8 n <= 64, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for the offset n = 1 */
    if (n == 1) {
        return 2;
    }

    /* Rational approximation 1/ln(2) ≈ 1442695 / 1000000  */
    /* Compute: floor((1442695 * n + 500000) / 1000000) = round(n/ln(2)) */
    idx_t numerator = (idx_t)1442695 * n + 500000;   /* add 0.5 * 1000000 for rounding */
    seq_t result = (seq_t)(numerator / 1000000);

    return result;
}

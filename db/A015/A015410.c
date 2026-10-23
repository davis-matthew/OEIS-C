/*
 * A015410  Gaussian binomial coefficient [ n,11 ] for q=-6.
 * https://oeis.org/A015410
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 11, i32 n <= 11, i64 n <= 13, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Product_{i=1..11} ((-6)^(n-i+1)-1)/((-6)^i-1)
 *   (Gaussian binomial coefficient [n,11] for q = -6)
 */
seq_t a(idx_t n)
{
    /* Return 1 for the first term (offset 11) */
    if (n == 11)
        return (seq_t)1;

    /* For n < 11 the sequence is not defined; return 0. */
    if (n < 11)
        return (seq_t)0;

    seq_t val = 1;
    seq_t pow_n    = 1;   /* (-6)^current_n  */
    seq_t pow_n_11 = 1;   /* (-6)^(current_n-11) */

    /* Compute (-6)^11 to initialise pow_n for n = 11 */
    for (idx_t i = 0; i < 11; ++i)
        pow_n *= (seq_t)-6;

    /* Iterate from n=12 up to the requested n */
    for (idx_t cur = 12; cur <= n; ++cur) {
        pow_n    *= (seq_t)-6;   /* (-6)^cur */
        pow_n_11 *= (seq_t)-6;   /* (-6)^(cur-11) */

        seq_t numer = pow_n - 1;
        seq_t denom = pow_n_11 - 1;

        val = val * numer / denom;
    }

    return val;
}

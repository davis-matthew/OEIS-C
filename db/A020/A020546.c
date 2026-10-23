/*
 * A020546  4th Bernoulli polynomial evaluated at x=n! (multiplied by 30).
 * https://oeis.org/A020546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 30 * (n!)^4 - 60 * (n!)^3 + 30 * (n!)^2 - 1
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 7, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute factorial n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* square the factorial */
    seq_t fact2 = fact * fact;

    /* cube and fourth power */
    seq_t fact3 = fact2 * fact;
    seq_t fact4 = fact2 * fact2;

    /* compute the polynomial 30*fact4 - 60*fact3 + 30*fact2 - 1 */
    seq_t result = (seq_t)30 * fact4;
    result -= (seq_t)60 * fact3;
    result += (seq_t)30 * fact2;
    result -= (seq_t)1;

    return result;
}

/*
 * A106691  Expansion of g.f. (1+x-2*x^2+x^3+x^4)/((1-x)^2*(1+x)^2*(1+2*x)^2).
 * https://oeis.org/A106691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (3*n + 4 - 27*(-1)^n*(n+4) + (-2)^(n+1)*(3*n - 79)) / 54
 * Range:     i8 n <= 0, i16 n <= 8, i32 n <= 27, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert index to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* (-1)^n as +1 or -1 */
    seq_t sign = ( (n & 1) == 0 ) ? (seq_t)1 : -(seq_t)1;

    /* Compute (-2)^(n+1) iteratively */
    seq_t pow_minus2 = (seq_t)1;
    for (idx_t i = 0; i < n + 1; ++i) {
        pow_minus2 *= (seq_t)(-2);
    }

    /* Component terms */
    seq_t term1 = (seq_t)3 * n_seq + (seq_t)4;
    seq_t term2 = -( (seq_t)27 * sign * (n_seq + (seq_t)4) );
    seq_t term3 = pow_minus2 * ( (seq_t)3 * n_seq - (seq_t)79 );

    /* Numerator and division */
    seq_t numerator   = term1 + term2 + term3;
    seq_t result = numerator / (seq_t)54;

    return result;
}

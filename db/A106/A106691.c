/*
 * A106691  Expansion of g.f. (1+x-2*x^2+x^3+x^4)/((1-x)^2*(1+x)^2*(1+2*x)^2).
 * https://oeis.org/A106691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (3*n + 4 - 27*(-1)^n*(n+4) + (-2)^(n+1)*(3*n - 79)) / 54
 * Range:     i8 n <= 5, i16 n <= 14, i32 n <= 31, i64 n <= 61, i128 n <= 122,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 1000 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

seq_t a_wide(idx_t n) {
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

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}

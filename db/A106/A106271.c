/*
 * A106271  Row sums of number triangle A106270.
 * https://oeis.org/A106271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -( (1-5*n)*a(n-1) + 2*(2*n-1)*a(n-2) )/(n+1), with a(0) =
 *            1, a(1) = 0. This follows from the recurrence stated in the OEIS
 *            entry for A106271
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 18, i64 n <= 35, i128 n <= 67,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 1659 (2026-11-03)
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

seq_t a_wide(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)0;

    seq_t a_nm2 = (seq_t)1; /* a_wide(0) */
    seq_t a_nm1 = (seq_t)0; /* a_wide(1) */

    for (idx_t i = 2u; i <= n; ++i) {
        /* term1 = (1-5*i) * a_wide(n-1) */
        seq_t term1 = ((seq_t)1 - (seq_t)5 * (seq_t)i) * a_nm1;

        /* term2 = 2*(2*i-1) * a_wide(n-2) */
        seq_t term2 = (seq_t)2 * ((seq_t)2 * (seq_t)i - (seq_t)1) * a_nm2;

        /* numerator = -(term1 + term2) */
        seq_t numerator = -(term1 + term2);

        /* denominator = i + 1 */
        seq_t denom = (seq_t)(i + 1u);

        seq_t ai = numerator / denom;

        a_nm2 = a_nm1;
        a_nm1 = ai;
    }

    return a_nm1;
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}

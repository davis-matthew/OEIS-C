/*
 * A168578  Coefficients of polynomials of the characteristic polynomials of
 *          two matrix systems subtracted: M(n)=Table[Table[If[m == k == 1, n,
 *          If[m == k, (-1)^n, 0]], {m, 1, n}], {k, 1, n}];M1(n)=Table[Table[
 *          If[m == k + 1, -1, If[k == n && m == 1, n, If[m == k == n, -n,
 *          0]]], {m, 1, n}], {k, 1, n}].
 * https://oeis.org/A168578
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, uned
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 56, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) is the n-th term of the sequence obtained by
 *           listing, for each matrix size N≥2, the coefficients of
 *           the polynomial
 *                Q_N(t) = (t−N)(t−(−1)^N)^(N−1) − [ (t+N)t^(N−1) + N(−1)^N ]
 *           for exponents e = 1,2,…,N−1 in ascending order.
 *           For odd N the coefficient is taken as its absolute value,
 *           for even N its signed value is kept.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* behaviour below offset is irrelevant */

    /* 1‑based index of the required coefficient within the concatenated list */
    idx_t coeff_idx = n - 1;

    /* Find matrix size N such that the cumulative number of coefficients
       up to N-1 is < coeff_idx ≤ cumulative up to N.  Since N is small
       (≤ √(2·coeff_idx)+1) a simple loop is sufficiently fast. */
    idx_t N = 2;
    while ((N - 1) * N / 2 < coeff_idx) ++N;

    /* Previous cumulative total before the current N-block */
    idx_t prev_T = (N - 2) * (N - 1) / 2;

    /* Exponent index e (1‑based) within the N‑block, 1 ≤ e ≤ N−1 */
    idx_t e = coeff_idx - prev_T;     /* 1 ≤ e ≤ N-1 */

    /* Binomial coefficients C(N−1, e−1) and C(N−1, e) */
    seq_t bin1 = binom(N - 1, e - 1);
    seq_t bin2 = binom(N - 1, e);

    /* Sign factors depending on the parity of N and e */
    int sign1 = 1;
    int sign2 = 1;
    if ((N & 1u) == 0) {           /* N even  ->  (-s) = -1 */
        sign1 = ((N - e) & 1u) ? -1 : 1;          /* (−1)^(N−e) */
        sign2 = ((N - 1 - e) & 1u) ? -1 : 1;      /* (−1)^(N−1−e) */
    }

    /* Coefficient of t^e in Q_N(t)  (signed, before absolute value) */
    seq_t coeffPM = (seq_t)sign1 * bin1 - (seq_t)N * (seq_t)sign2 * bin2;
    if (e == N - 1) coeffPM -= (seq_t)N;   /* subtract N from e=N−1 term */

    /* For odd N we output the absolute value */
    if ((N & 1u) == 1) {
        if (coeffPM < 0) return -coeffPM;
        return coeffPM;
    }
    return coeffPM;
}

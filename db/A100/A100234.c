/*
 * A100234  G.f. A(x) satisfies: 6^n - 1 = Sum_{k=0..n} [x^k]A(x)^n and also
 *          satisfies: (6+z)^n - (1+z)^n + z^n = Sum_{k=0..n}
 *          [x^k](A(x)+z*x)^n for all z, where [x^k]A(x)^n denotes the
 *          coefficient of x^k in A(x)^n.
 * https://oeis.org/A100234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -(3*(2*n-3)*a(n-1)+29*(n-3)*a(n-2))/n for n>2 with
 *            a(0)=1, a(1)=4, a(2)=5
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 14, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)4;
    if (n == 2u) return (seq_t)5;

    /* a_prev2 = a(n-2), a_prev1 = a(n-1) for the current n */
    seq_t a_prev2 = (seq_t)4;   /* a(1) */
    seq_t a_prev1 = (seq_t)5;   /* a(2) */
    seq_t a_n     = a_prev1;    /* will hold a(n) */

    for (idx_t i = 3u; i <= n; ++i) {
        seq_t term1 = (seq_t)3 * (seq_t)(2 * i - 3) * a_prev1;
        seq_t term2 = (seq_t)29 * (seq_t)(i - 3) * a_prev2;
        seq_t numerator = -(term1 + term2);
        a_n = numerator / (seq_t)i;

        a_prev2 = a_prev1;
        a_prev1 = a_n;
    }

    return a_n;
}

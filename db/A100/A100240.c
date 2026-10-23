/*
 * A100240  G.f. A(x) satisfies: 4^n/2 = Sum_{k=0..n} [x^k]A(x)^n and also
 *          satisfies: ((4+z)^n + z^n)/2 = Sum_{k=0..n} [x^k](A(x)+z*x)^n for
 *          all z, where [x^k]A(x)^n denotes the coefficient of x^k in A(x)^n.
 * https://oeis.org/A100240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 30, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: n*a(n) = (2*n-3)*a(n-1) - 5*(n-3)*a(n-2)
   Initial values: a(0)=1, a(1)=1, a(2)=2 */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)2;

    seq_t a_prev2 = (seq_t)1;  /* a(0) */
    seq_t a_prev1 = (seq_t)1;  /* a(1) */
    seq_t a_cur   = (seq_t)2;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t part1 = ((seq_t)2 * (seq_t)i - (seq_t)3) * a_cur;
        seq_t part2 = (seq_t)5 * (seq_t)(i - 3) * a_prev1;
        seq_t ai = (part1 - part2) / (seq_t)i;

        a_prev2 = a_prev1;
        a_prev1 = a_cur;
        a_cur   = ai;
    }

    return a_cur;
}

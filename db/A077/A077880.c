/*
 * A077880  Expansion of (1-x)^(-1)/(1-2*x^2+x^3).
 * https://oeis.org/A077880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*a(n-2) - a(n-3) + 1 with initial values a(0) = 1, a(1)
 *            = 1, a(2) = 3. This recursive definition matches a(n) =
 *            (-1)^n*Fibonacci(n-1) + n for all n ≥ 0
 * Range:     i8 n <= 12, i16 n <= 24, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* Three consecutive terms needed: a[n-3], a[n-2], a[n-1] */
    seq_t v0 = (seq_t)1;   /* a(n-3) */
    seq_t v1 = (seq_t)1;   /* a(n-2) */
    seq_t v2 = (seq_t)3;   /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 2*a(i-2) - a(i-3) + 1 */
        seq_t cur = 2 * v1 - v0 + (seq_t)1;
        v0 = v1;
        v1 = v2;
        v2 = cur;
    }
    return v2; /* a(n) */
}

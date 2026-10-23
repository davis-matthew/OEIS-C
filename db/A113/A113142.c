/*
 * A113142  Expansion of x(1-3x+x^2+x^3)/(1+x)^2.
 * https://oeis.org/A113142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -(-1)^n*(4n-2-C(1,n-1)+2*C(0,n))
 * Range:     i8 n <= 32, i16 n <= 47, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n ≥ 3 the formula simplifies to a(n) = ((n&1)?1:-1)*(4n-2) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-5;

    seq_t val = (seq_t)4 * (seq_t)n - (seq_t)2;   /* 4n - 2 */
    /* n odd → +, n even → - */
    if ((n & 1) == 0)
        return -val;
    else
        return val;
}

/*
 * A091096  Expansion of (1+5x-40x^2)/((1-5x)(1+5x)).
 * https://oeis.org/A091096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 5^n/5 + (-4)(-5)^n/5 + 8*0^n/5
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _pow5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
/* Equivalent: a(0)=1; for n≥1: a(n)=5^n if n is odd,  a(n)=−3·5^(n−1) if n is even. */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    if (n & 1)                /* n is odd */
        return _pow5(n);      /* 5^n */

    /* n is even; a(n) = −3·5^(n−1) */
    seq_t v = _pow5(n - 1);         /* 5^(n−1) */
    return v * ((seq_t)-3);         /* multiply by −3 */
}

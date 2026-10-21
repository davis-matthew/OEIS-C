/*
 * A099570  Expansion of ((1+x)^2 - x^3)/(1+x)^2.
 * https://oeis.org/A099570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=0, a(n)=(-1)^n*(n-2) for n>=2
 * Range:     i8 n <= 129, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements A099570 directly */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;
    /* For n>=2 */
    seq_t sign = (n & 1) ? (seq_t)-1 : (seq_t)1;
    return sign * (((seq_t)n) - (seq_t)2);
}

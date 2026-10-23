/*
 * A122434  Expansion of (1+x)^3/(1+x+x^2).
 * https://oeis.org/A122434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 1 for n=0, * a(n) = 2 for n=1, * a(n) = 0 if n%3==2, *
 *            a(n) = -1 if n%3==0, * a(n) = 1 if n%3==1. (Derived from
 *            (1+x)^3/(1+x+x^2))
 * Range:     i8 n <= 68, i16 n <= 68, i32 n <= 68, i64 n <= 68, i128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    int r = (int)(n % 3);
    if (r == 0) return (seq_t)-1;
    if (r == 1) return (seq_t)1;
    return (seq_t)0;   /* r == 2 */
}

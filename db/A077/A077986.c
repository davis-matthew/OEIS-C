/*
 * A077986  Expansion of 1/(1 + 2*x - x^2 + x^3).
 * https://oeis.org/A077986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) + a(n-2) - a(n-3) with initial values a(0)=1,
 *            a(1)=-2, a(2)=5
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 23, i64 n <= 46, i128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t)5;

    seq_t a0 = (seq_t)1;   // a(n-3)
    seq_t a1 = (seq_t)-2;  // a(n-2)
    seq_t a2 = (seq_t)5;   // a(n-1)
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = -2 * a2 + a1 - a0;  // a(n)
        a0 = a1;                 // slide window
        a1 = a2;
        a2 = ai;
    }
    return a2;  // a(n)
}

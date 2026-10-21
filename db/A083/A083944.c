/*
 * A083944  A generalized Jacobsthal sequence.
 * https://oeis.org/A083944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - 3, with a(0) = 0, a(1) = 1
 *            (equivalently a(n) = (9 - 2^(n+2) - 5*(-1)^n)/6)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t a0 = 0;                 /* a(n-2) for current n */
    seq_t a1 = 1;                 /* a(n-1) for current n */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t a2 = a1 + (seq_t)2 * a0 - (seq_t)3; /* a(n) */
        a0 = a1;
        a1 = a2;
    }
    return a1;
}

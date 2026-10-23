/*
 * A159486  Numerator of Hermite(n, 11/12).
 * https://oeis.org/A159486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 11*a(n-1) - 72*(n-1)*a(n-2) initial values: a(0) = 1,
 *            a(1) = 11
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 14, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 11;

    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = 11;  /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = 11 * prev1;                 /* 11 * a(i-1) */
        seq_t term = prev2 * (seq_t)(i - 1);/* (i-1) * a(i-2) */
        term = term * 72;                  /* 72 * (i-1) * a(i-2) */
        curr = curr - term;                /* a(i) */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}

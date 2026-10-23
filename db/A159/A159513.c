/*
 * A159513  Numerator of Hermite(n, 1/15).
 * https://oeis.org/A159513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 15^n * Hermite(n, 1/15) Recurrence: a(0) = 1, a(1) = 2
 *            a(n) = 2*a(n-1) - 450*(n-1)*a(n-2) for n >= 2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)2;   /* a(1) */
    seq_t curr;

    for (idx_t i = 1; i < n; ++i) {
        /* i == index of prev1 (a(i)) ; compute a(i+1) */
        seq_t coeff = (seq_t)450 * (seq_t)i;   /* 450 * i */
        curr = 2 * prev1;                      /* 2*a(i) */
        curr -= coeff * prev2;                 /* -450*i*a(i-1) */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}

/*
 * A159543  Numerator of Hermite(n, 15/17).
 * https://oeis.org/A159543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 30*a(n-1) - 578*(n-1)*a(n-2) with a(0)=1, a(1)=30
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 11, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 30;

    seq_t prev2 = 1;     /* a(n-2) */
    seq_t prev1 = 30;    /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1 = prev1 * 30;                    /* 30*a(n-1) */
        seq_t multiplier = (seq_t)578 * (i - 1);      /* 578*(n-1) */
        seq_t term2 = prev2 * multiplier;            /* 578*(n-1)*a(n-2) */
        seq_t curr = term1 - term2;                  /* a(n) */

        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}

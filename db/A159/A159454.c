/*
 * A159454  Numerator of Hermite(n, 8/11).
 * https://oeis.org/A159454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 16*a(n-1) - 242*(n-1)*a(n-2); a(0)=1, a(1)=16
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;
    if (n == (idx_t)1)
        return (seq_t)16;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)16;  /* a(1) */
    seq_t curr;

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        curr = prev1 * (seq_t)16;                /* 16*a(i-1) */
        idx_t mult = (idx_t)242 * (i - (idx_t)1); /* 242*(i-1) */
        curr = curr - (prev2 * (seq_t)mult);      /* minus 242*(i-1)*a(i-2) */
        /* shift for next iteration */
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

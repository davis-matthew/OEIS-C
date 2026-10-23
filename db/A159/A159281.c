/*
 * A159281  Numerator of Hermite(n, 2/11).
 * https://oeis.org/A159281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=4, a(n)=4*a(n-1)-242*(n-1)*a(n-2) for n≥2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)4;
    seq_t prev2 = (seq_t)1; /* a(0) */
    seq_t prev1 = (seq_t)4; /* a(1) */
    seq_t curr;
    for (idx_t i = 2u; i <= n; ++i) {
        seq_t term = prev2 * (seq_t)(i - 1u); /* (n-1)*a(n-2) */
        term = term * (seq_t)242;            /* 242*(n-1)*a(n-2) */
        curr = prev1 * (seq_t)4 - term;      /* 4*a(n-1)-term */
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

/*
 * A128415  Expansion of (1-4x^2)/(1+3x+4x^2).
 * https://oeis.org/A128415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -3*a(n-1) - 4*a(n-2) for n > 2; a(0) = 1, a(1) = -3, a(2)
 *            = 1
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 30, i64 n <= 63, i128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)1;
    if (n == 1ULL) return (seq_t)(-3);
    if (n == 2ULL) return (seq_t)1;

    /* a(n) depends only on the two previous terms, so we can compute
       iteratively without any dynamic allocation. */
    seq_t prev2 = (seq_t)(-3);   /* a(1)  */
    seq_t prev1 = (seq_t)1;      /* a(2)  */
    seq_t cur = 0;

    for (idx_t i = 3ULL; i <= n; ++i) {
        cur = (seq_t)(-3) * prev1 - (seq_t)(4) * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

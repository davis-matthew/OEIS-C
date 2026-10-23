/*
 * A133511  a(n) = 3 A113405(n)- A113405(n+1).
 * https://oeis.org/A133511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 34, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*A113405(n) - A113405(n+1).
 *
 * Recurrence derived from the generating function:
 *   a(0) = 0,
 *   a(1) = 0,
 *   a(2) = -1,
 *   a(3) = 1,
 *   for n ≥ 4:
 *       a(n) = 2*a(n-1) - a(n-3) + 2*a(n-4).
 *
 * The function computes the value iteratively, keeping only the last
 * four terms of the sequence to avoid any memory allocation or
 * caching.
 */

seq_t a(idx_t n)
{
    /* Base values */
    seq_t a0 = (seq_t)0;        /* a(0) */
    seq_t a1 = (seq_t)0;        /* a(1) */
    seq_t a2 = (seq_t)-1;       /* a(2) */
    seq_t a3 = (seq_t)1;        /* a(3) */

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    /* Array holding the previous four values:
     * prev[0] = a(n-4), prev[1] = a(n-3), prev[2] = a(n-2), prev[3] = a(n-1)
     */
    seq_t prev[4] = {a0, a1, a2, a3};
    seq_t cur;   /* current value a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        cur = 2 * prev[3] - prev[1] + 2 * prev[0];  /* a(i) = 2*a(i-1) - a(i-3) + 2*a(i-4) */
        /* shift values for next iteration */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = cur;
    }

    return cur;
}

/*
 * A080559  Duplicate of A010928.
 * https://oeis.org/A080559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = binomial(12, n) for 0 ≤ n ≤ 12, otherwise 0
 * Range:     i8 n <= 2, i16 n <= 12, i32 n <= 12, i64 n <= 12, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t N = 12;          /* fixed row of Pascal's triangle   */
    if (n > N)                   /* terms beyond the 12th vanish    */
        return 0;

    /* Use symmetry to reduce the number of multiplications      */
    idx_t k = n;
    if (k > N - k)
        k = N - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (seq_t)(N - k + i) / (seq_t)i;  /* exact each step */

    return res;
}

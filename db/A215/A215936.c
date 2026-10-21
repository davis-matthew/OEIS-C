/*
 * A215936  a(n) = -2*a(n-1) + a(n-2) for n > 2, with a(0) = a(1) = 1, a(2) =
 *          0.
 * https://oeis.org/A215936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) + a(n-2) for n > 2 a(0) = a(1) = 1, a(2) = 0
 * Range:     i8 n <= 8, i16 n <= 14, i32 n <= 27, i64 n <= 52, i128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)0;

    seq_t prev2 = (seq_t)1;  /* a(1) */
    seq_t prev1 = (seq_t)0;  /* a(2) */
    seq_t ai = (seq_t)0;

    for (idx_t i = 3u; i <= n; ++i) {
        ai = - (seq_t)2 * prev1 + prev2;
        prev2 = prev1;
        prev1 = ai;
    }
    return ai;
}

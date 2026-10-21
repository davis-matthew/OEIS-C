/*
 * A083581  a(n) = 8/3 - 5*(-2)^n/3.
 * https://oeis.org/A083581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 62, i128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrent): a(0) = 1,
   a(n) = −2·a(n−1) + 8  (for n ≥ 1)     */
/* This recurrence follows from a(n) = (8 − 5(−2)^n)/3. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t val = (seq_t)1;         /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Compute a(i) = −2·a(i−1) + 8 */
        val = - (seq_t)2 * val + (seq_t)8;
    }
    return val;
}

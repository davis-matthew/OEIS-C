/*
 * A227589  Maximum label within a minimal labeling of n identical 4-sided
 *          dice yielding the most possible sums.
 * https://oeis.org/A227589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 + 3n + 5 - (-1)^n)/2 for n > 1 Special cases: a(0) =
 *            1, a(1) = 4
 * Range:     u8 n <= 14, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t ns = (seq_t)n;
    seq_t t  = ns * ns + (seq_t)3 * ns + (seq_t)5; /* n^2 + 3n + 5 */

    if (n & 1)         /* n odd : subtract (-1)^n = -1 → add 1 */
        t += (seq_t)1;
    else               /* n even: subtract 1 */
        t -= (seq_t)1;

    return t / (seq_t)2;
}

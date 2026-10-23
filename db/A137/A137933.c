/*
 * A137933  Least common multiple of n^2 and 2.
 * https://oeis.org/A137933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n^2, 2)
 * Range:     u8 n <= 12, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;                 /* behaviour below offset is irrelevant */

    seq_t v  = (seq_t)n;          /* convert index to seq_t */
    seq_t v2 = v * v;             /* n^2 */

    /* if n is odd, lcm(n^2,2) = 2 * n^2; otherwise it is n^2 */
    if ((n & 1u) != 0u)
        return v2 * 2;            /* 2 * n^2 */
    else
        return v2;                /* n^2 */
}

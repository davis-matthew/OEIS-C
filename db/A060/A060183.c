/*
 * A060183  a(0)=1, a(n) = 100*a(n-1) + 36*n - 128.
 * https://oeis.org/A060183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*a(n-1) + 36*n - 128, a(0) = 1 (Store the sequence in
 *            a cache is unnecessary; the recurrence can be evaluated in O(n)
 *            time for each call, which is fast enough for n up to about 10
 *            000 even with an arbitrary‑precision integer type.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;                 /* a(0) */
    if (n == 0)
        return result;

    /* Compute a(1) through a(n) iteratively. */
    for (idx_t i = 1; i <= n; ++i)
    {
        result = 100 * result;        /* 100 * a(i-1) */
        result += 36 * i;             /* + 36*i */
        result -= 128;                /* - 128 */
    }
    return result;
}

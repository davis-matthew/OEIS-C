/*
 * A091045  Partial sums of powers of 17 (A001026).
 * https://oeis.org/A091045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*17 + 1, with a(1) = 1. (Partial sums of 17^k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; 0 is not used, return 0 safely */
        return (seq_t)0;

    seq_t val = 1;        /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        val = val * (seq_t)17 + (seq_t)1;   /* recurrence */
    }
    return val;
}

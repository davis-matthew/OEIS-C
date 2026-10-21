/*
 * A067272  a(n) = 2*10^(n-1) - 1.
 * https://oeis.org/A067272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*10^(n-1) - 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, but we guard against n==0 for safety. */
    if (n == 0) return (seq_t)0;
    seq_t val = (seq_t)2;           /* start with 2*10^0 */
    for (idx_t i = 1; i < n; ++i)   /* multiply by 10 (n-1) times */
        val *= (seq_t)10;
    val -= (seq_t)1;                /* subtract 1 to get the term */
    return val;
}

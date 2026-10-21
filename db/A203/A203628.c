/*
 * A203628  Indices of 9-gonal (nonagonal) numbers which are also 10-gonal
 *          (decagonal).
 * https://oeis.org/A203628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 898*a(n-1) - a(n-2) - 320 Initial values: a(1) = 1, a(2)
 *            = 589
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined for offset>1 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)589;

    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)589; /* a(2) */
    seq_t curr  = prev1;      /* placeholder */

    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)898 * prev1 - prev2 - (seq_t)320;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}

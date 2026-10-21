/*
 * A081036  9th binomial transform of the periodic sequence
 *          (1,10,1,1,10,1...).
 * https://oeis.org/A081036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 80*a(n-2), a(0) = 1, a(1) = 19
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)19;

    /* a(0) and a(1) as initial values */
    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = 19;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 18 * prev1 - 80 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

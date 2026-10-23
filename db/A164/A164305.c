/*
 * A164305  a(n) = 6*a(n-1) - 7*a(n-2) for n > 1; a(0) = 3, a(1) = 17.
 * https://oeis.org/A164305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2), a(0) = 3, a(1) = 17
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 3;        /* a(0) */
    if (n == 1) return 17;       /* a(1) */

    seq_t prev2 = 3;             /* a(0) */
    seq_t prev1 = 17;            /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 6 * prev1 - 7 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

/*
 * A155199  a(n) = 10*a(n-1) + a(n-2), n > 2 ; a(0) = 1, a(1) = 9, a(2) = 90.
 * https://oeis.org/A155199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-2) for n > 2; a(0) = 1, a(1) = 9, a(2) =
 *            90
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)90;

    /* Iterative computation for n ≥ 3 */
    seq_t prev2 = (seq_t)9;   /* a(1) */
    seq_t prev1 = (seq_t)90;  /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = ((seq_t)10 * prev1) + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

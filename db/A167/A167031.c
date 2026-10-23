/*
 * A167031  a(n) = 20*a(n-1) - 64*a(n-2) + 1 for n > 1; a(0) = 1, a(1) = 20.
 * https://oeis.org/A167031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 64*a(n-2) + 1, a(0) = 1, a(1) = 20
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)20;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = ((seq_t)20 * prev1) - ((seq_t)64 * prev2) + (seq_t)1;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

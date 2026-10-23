/*
 * A048905  Indices of octagonal numbers which are also heptagonal.
 * https://oeis.org/A048905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 482*a(n-1) - a(n-2) - 160 with a(1) = 1, a(2) = 315
 *            (offset 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)315;
    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)315; /* a(2) */
    for (idx_t i = 3u; i <= n; ++i) {
        seq_t cur = prev1 * (seq_t)482 - prev2 - (seq_t)160;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

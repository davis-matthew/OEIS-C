/*
 * A060531  9th binomial transform of (1,0,1,0,1,...), A059841.
 * https://oeis.org/A060531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8^n + 10^n)/2, equivalently a(0)=1, a(1)=9, a(n) =
 *            18*a(n-1) - 80*a(n-2) for n >= 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)9;   /* a(1) */
    seq_t curr;
    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)18 * prev1 - (seq_t)80 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

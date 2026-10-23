/*
 * A026581  Expansion of (1 + 2*x) / (1 - x - 4*x^2).
 * https://oeis.org/A026581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=3; a(n)=a(n-1)+4*a(n-2) for n>1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)3;  /* a(1) */
    seq_t cur    = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 + prev2 * (seq_t)4;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}

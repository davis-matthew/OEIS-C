/*
 * A154964  a(n) = 3*a(n-1) + 6*a(n-2), n>2, a(0)=1, a(1)=1, a(2)=5.
 * https://oeis.org/A154964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=5; a(n)=3*a(n‑1)+6*a(n‑2) for n>2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    seq_t prev2 = (seq_t)1;  /* a(1) */
    seq_t prev1 = (seq_t)5;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = (seq_t)3 * prev1 + (seq_t)6 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}

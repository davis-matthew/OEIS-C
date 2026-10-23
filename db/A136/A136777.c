/*
 * A136777  Number of multiplex juggling sequences of length n, base state
 *          <2,1> and hand capacity 2.
 * https://oeis.org/A136777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 13*a(n-2) with a(1)=1, a(2)=4, a(3)=22
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, return 0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)22;

    seq_t prev2 = (seq_t)4;   /* a(2) */
    seq_t prev1 = (seq_t)22;  /* a(3) */
    seq_t cur;
    for (idx_t i = 4; i <= n; ++i) {
        cur = prev1 * (seq_t)8 - prev2 * (seq_t)13;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

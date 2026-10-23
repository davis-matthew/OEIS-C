/*
 * A020704  Pisot sequences E(3,10), P(3,10).
 * https://oeis.org/A020704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) a(0) = 3, a(1) = 10
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first two terms explicitly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)10;

    /* iterate the recurrence up to n */
    seq_t prev2 = (seq_t)3;   /* a(0) */
    seq_t prev1 = (seq_t)10;  /* a(1) */
    idx_t i;

    for (i = 2; i <= n; ++i) {
        seq_t cur = (seq_t)3 * prev1 + prev2;  /* a(i) = 3*a(i-1) + a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}

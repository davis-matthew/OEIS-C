/*
 * A065930  (x,y) = (a(n),a(n+1)) are the solutions of (t(x)+t(y))/(1+xy) =
 *          t(4) = 10, where t(n) denotes the n-th triangular number t(n) =
 *          n*(n+1)/2.
 * https://oeis.org/A065930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 20*a(n-1) - a(n-2) - 1; a(0) = 4; a(1) = 79
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)79;

    seq_t a0 = (seq_t)4;
    seq_t a1 = (seq_t)79;
    idx_t i;

    for (i = 2; i <= n; ++i) {
        seq_t a2 = ((seq_t)20 * a1) - a0 - (seq_t)1;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}

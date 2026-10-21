/*
 * A209721  1/4 the number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock
 *          having distinct clockwise edge differences.
 * https://oeis.org/A209721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2)-1 for n ≥ 3; a(1) = 3, a(2) = 4
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A209721 (sequence of 1/4  (n+1)×3 arrays) */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;

    seq_t prev2 = (seq_t)3;   /* a(i-2) */
    seq_t prev1 = (seq_t)4;   /* a(i-1) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = prev2 * 2;
        cur -= (seq_t)1;            /* a(i) = 2*a(i-2) - 1 */
        prev2 = prev1;              /* shift window */
        prev1 = cur;
    }
    return prev1;
}

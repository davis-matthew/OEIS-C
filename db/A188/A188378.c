/*
 * A188378  Partial sums of A005248.
 * https://oeis.org/A188378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000032(2*n+1)+1 (Lucas number L(2n+1)+1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute m = 2*n+1 (Lucas index) */
    idx_t m = 2*n + 1;

    /* Base Lucas numbers: L(0)=2, L(1)=1 */
    if (m == 0) return (seq_t)3;          /* 2 + 1, not used for n>=0 */
    if (m == 1) return (seq_t)2;          /* 1 + 1 */

    seq_t prev = (seq_t)2;    /* L(0) */
    seq_t curr = (seq_t)1;    /* L(1) */

    for (idx_t i = 2; i <= m; ++i)
    {
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }
    /* curr now holds L(2*n+1) */
    return curr + (seq_t)1;
}

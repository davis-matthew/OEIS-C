/*
 * A048777  First partial sums of A005409; second partial sums of A001333.
 * https://oeis.org/A048777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2*n + 1 with a(0)=1, a(1)=5
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* recurrence variables */
    seq_t prev2 = (seq_t)1;  /* a(0)   */
    seq_t prev1 = (seq_t)5;  /* a(1)   */
    seq_t cur;

    /* iterate from 2 up to n */
    for (idx_t i = 2; i <= n; ++i) {
        /* cur = 2*a(i-1) + a(i-2) + 2*i + 1 */
        cur = (seq_t)2 * prev1 + prev2 + (seq_t)2 * (seq_t)i + (seq_t)1;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

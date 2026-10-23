/*
 * A133680  a(n)=8*a(n-1)+72*a(n-2) for n>=3, a(0)=1, a(1)=8, a(2)=128 .
 * https://oeis.org/A133680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1)+72*a(n-2) for n>=3, a(0)=1, a(1)=8, a(2)=128
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)128;

    seq_t prev2 = (seq_t)8;   /* a(1) */
    seq_t prev1 = (seq_t)128; /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)8 * prev1 + (seq_t)72 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

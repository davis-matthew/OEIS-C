/*
 * A147672  a(n) = a(n-2) + 2^(n-1) + 5 for n>3, a(0..3) = (0,1,2,7).
 * https://oeis.org/A147672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2^(n-1) + 5 for n>3, a(0)=0, a(1)=1, a(2)=2,
 *            a(3)=7 (OEIS A147672)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)7;

    /* a(n-2) and a(n-1) for the next iteration */
    seq_t prev2 = (seq_t)2;   /* a(2) */
    seq_t prev1 = (seq_t)7;   /* a(3) */
    seq_t curr;

    /* 2^(i-1) for i=4 starts at 2^3 = 8 */
    seq_t pow2 = (seq_t)1 << 3;

    for (idx_t i = 4; i <= n; ++i) {
        curr = prev2 + pow2 + (seq_t)5;
        pow2 <<= 1;          /* next power of two */
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

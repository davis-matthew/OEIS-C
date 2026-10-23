/*
 * A147959  a(n) = ((8 + sqrt(2))^n + (8 - sqrt(2))^n)/2.
 * https://oeis.org/A147959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - 62*a(n-2), with a(0)=1, a(1)=8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)8;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)8;   /* a(1) */
    seq_t cur   = prev1;

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        cur = (seq_t)16 * prev1 - (seq_t)62 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

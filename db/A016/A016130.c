/*
 * A016130  Expansion of g.f. 1/((1-2*x)*(1-7*x)).
 * https://oeis.org/A016130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + 2^n, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prev = (seq_t)1;      /* a(0) */
    if (n == 0) return prev;

    seq_t pow2 = (seq_t)2;      /* 2^1 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t curr = (seq_t)7 * prev + pow2;
        if (i == n) return curr;
        prev = curr;
        pow2 <<= 1;              /* next power of two */
    }
    return prev;                 /* unreachable, but keeps compiler happy */
}

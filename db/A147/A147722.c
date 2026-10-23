/*
 * A147722  Row sums of Riordan array ((1-3x)/(1-4x+x^2), x(1-x)/(1-4x+x^2)).
 * https://oeis.org/A147722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(n)=5*a(n-1)-2*a(n-2) for n>=2 */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;
    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)2;
    seq_t cur;
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        cur = (seq_t)5 * prev1 - (seq_t)2 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

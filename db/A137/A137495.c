/*
 * A137495  a(n) = A098601(2n) + A098601(2n+1).
 * https://oeis.org/A137495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) with a(0)=2, a(1)=3, a(2)=4
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;

    seq_t prev2 = (seq_t)2;   /* a(0) */
    seq_t prev1 = (seq_t)3;   /* a(1) */
    seq_t curr  = (seq_t)4;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)2 * curr;  /* 2*a(n-1) */
        next -= prev1;                 /* -a(n-2) */
        next += prev2;                 /* +a(n-3) */
        prev2 = prev1;
        prev1 = curr;
        curr  = next;
    }
    return curr;
}

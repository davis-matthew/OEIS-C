/*
 * A156161  a(n) = 34*a(n-1)-a(n-2)-2312 for n > 2; a(1)=289, a(2)=7225.
 * https://oeis.org/A156161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=289, a(2)=7225; a(n) = 34*a(n-1)-a(n-2)-2312 for n>2 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, return 0 */
    if (n == 1) return (seq_t)289;
    if (n == 2) return (seq_t)7225;

    seq_t prev2 = (seq_t)289;
    seq_t prev1 = (seq_t)7225;
    seq_t next;
    for (idx_t i = 3; i <= n; ++i) {
        next = prev1 * (seq_t)34 - prev2 - (seq_t)2312;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}

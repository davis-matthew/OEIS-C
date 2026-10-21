/*
 * A053455  a(n) = ((8^n) - (-6)^n)/14.
 * https://oeis.org/A053455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((8^n)-(-6)^n)/14 Recurrence: a(0)=0, a(1)=1,
 *            a(n)=2*a(n-1)+48*a(n-2) for n>=2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    seq_t prev2 = 0;   /* a(0) */
    seq_t prev1 = 1;   /* a(1) */
    idx_t i;
    for (i = 2; i <= n; ++i) {
        seq_t next = 2 * prev1 + 48 * prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}

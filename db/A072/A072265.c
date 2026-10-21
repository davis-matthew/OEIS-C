/*
 * A072265  Variant of Lucas numbers: a(n) = a(n-1) + 4*a(n-2) starting with
 *          a(0)=2 and a(1)=1.
 * https://oeis.org/A072265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2), a(0)=2, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)2; /* a(0) */
    seq_t prev1 = (seq_t)1; /* a(1) */
    seq_t cur = prev1;      /* placeholder, will be overwritten */

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 + (prev2 << 2); /* a(i) = a(i-1) + 4*a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

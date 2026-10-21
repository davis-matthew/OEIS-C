/*
 * A055271  a(n) = 5*a(n-1) - a(n-2) with a(0)=1, a(1)=7.
 * https://oeis.org/A055271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - a(n-2), a(0)=1, a(1)=7 Iterative computation,
 *            no dynamic allocation, works for all seq_t (unsigned integers or
 *            arbitrary‑precision types)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)7;   /* a(n-1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)5 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

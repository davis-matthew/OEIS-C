/*
 * A083231  a(n) = (3*5^n + (-3)^n)/4.
 * https://oeis.org/A083231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*5^n + (-3)^n)/4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t prev2 = (seq_t)1; /* a(0) */
    seq_t prev1 = (seq_t)3; /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + 15*a(i-2) */
        cur = (seq_t)2 * prev1 + (seq_t)15 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

/*
 * A089979  Expansion of 1/(1-4x-4x^3).
 * https://oeis.org/A089979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-3), a(0)=1, a(1)=4, a(2)=16
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)16;

    seq_t prev3 = (seq_t)1;   /* a(n-3) */
    seq_t prev2 = (seq_t)4;   /* a(n-2) */
    seq_t prev1 = (seq_t)16;  /* a(n-1) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)4 * prev1 + (seq_t)4 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}

/*
 * A084130  a(n) = 8*a(n-1) - 8*a(n-2), a(0)=1, a(1)=4.
 * https://oeis.org/A084130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2), a(0) = 1, a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)4;

    seq_t prev2 = (seq_t)1; /* a(n-2) */
    seq_t prev1 = (seq_t)4; /* a(n-1) */
    seq_t curr;

    for (idx_t i = 2u; i <= n; ++i) {
        curr = (seq_t)8 * prev1 - (seq_t)8 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

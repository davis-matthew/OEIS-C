/*
 * A164553  a(n) = 14*a(n-1)-43*a(n-2) for n > 1; a(0) = 1, a(1) = 11.
 * https://oeis.org/A164553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 43*a(n-2) for n > 1; a(0)=1, a(1)=11
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t a0 = (seq_t)1;
    const seq_t a1 = (seq_t)11;
    if (n == 0)
        return a0;
    if (n == 1)
        return a1;
    seq_t prev2 = a0;
    seq_t prev1 = a1;
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)14 * prev1 - (seq_t)43 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

/*
 * A185561  1/4 the number of n X 3 0..3 arrays with no element equal both to
 *          the element above and to the element to its left.
 * https://oeis.org/A185561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 57*a(n-1) - 45*a(n-2) (a(1)=16, a(2)=900)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 16;
    if (n == 2) return 900;
    seq_t prev2 = 16; /* a(1) */
    seq_t prev1 = 900; /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = 57 * prev1 - 45 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

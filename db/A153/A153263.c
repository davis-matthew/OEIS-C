/*
 * A153263  a(n) = A014217(n+3) - A014217(n).
 * https://oeis.org/A153263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + a(n-3); a(0)=3, a(1)=5, a(2)=9
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 3;
    if (n == 1) return 5;
    if (n == 2) return 9;

    seq_t prev3 = 3; /* a[0] */
    seq_t prev2 = 5; /* a[1] */
    seq_t prev1 = 9; /* a[2] */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)2 * prev2 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

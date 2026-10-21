/*
 * A076740  a(n) = 2*a(n-1)^2 - a(n-2)^2 with a(0)=0, a(1)=1.
 * https://oeis.org/A076740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, a(n) = 2*a(n-1)^2 - a(n-2)^2
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev2 = 0;  /* a(n-2) */
    seq_t prev1 = 1;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 2 * prev1 * prev1 - prev2 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

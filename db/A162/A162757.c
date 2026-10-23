/*
 * A162757  a(n) = 12*a(n-1)-33*a(n-2) for n > 1; a(0) = 1, a(1) = 7.
 * https://oeis.org/A162757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1)-33*a(n-2) for n > 1; a(0) = 1, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 7;

    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = 7;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = 12 * prev1 - 33 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

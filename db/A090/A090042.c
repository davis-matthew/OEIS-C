/*
 * A090042  a(n) = 2*a(n-1) + 11*a(n-2) for n > 1, a(0) = a(1) = 1.
 * https://oeis.org/A090042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 11*a(n-2), a(0) = a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t prev2 = (seq_t)1; /* a(0) */
    seq_t prev1 = (seq_t)1; /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * 2 + prev2 * 11;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}

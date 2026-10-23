/*
 * A171480  a(n) = 6*a(n-1) - 8*a(n-2) + 4 for n > 1; a(0) = 1, a(1) = 9.
 * https://oeis.org/A171480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 8*a(n-2) + 4 for n > 1; a(0) = 1, a(1) = 9;
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)9;

    seq_t prevPrev = (seq_t)1;   /* a(0) */
    seq_t prev     = (seq_t)9;   /* a(1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        next = (seq_t)6 * prev - (seq_t)8 * prevPrev + (seq_t)4;
        prevPrev = prev;
        prev = next;
    }
    return prev;
}

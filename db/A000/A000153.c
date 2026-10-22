/*
 * A000153  a(n) = n*a(n-1) + (n-2)*a(n-2), with a(0) = 0, a(1) = 1.
 * https://oeis.org/A000153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*a(n-1) + (n-2)*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prev = 0;   /* a(i-2) */
    seq_t cur = 1;    /* a(i-1) */
    idx_t i;

    if (n == 0)
        return prev;
    for (i = 2; i <= n; i++) {
        seq_t next = (seq_t)i * cur + (seq_t)(i - 2) * prev;
        prev = cur;
        cur = next;
    }
    return cur;
}

/*
 * A015446  Generalized Fibonacci numbers: a(n) = a(n-1) + 10*a(n-2).
 * https://oeis.org/A015446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 10*a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) {
        return (seq_t)1;
    }
    seq_t prev = (seq_t)1; /* a(0) */
    seq_t cur  = (seq_t)1; /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = cur + (seq_t)10 * prev;
        prev = cur;
        cur = next;
    }
    return cur;
}

/*
 * A028300  a(1) = 1; thereafter a(n+1) = a(n)^2 + n.
 * https://oeis.org/A028300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; thereafter a(n+1) = a(n)^2 + n for n ≥ 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = 1;          /* a(1) */
    if (n <= 1) return cur; /* offset handling */

    for (idx_t i = 1; i < n; ++i) {
        /* compute a(i+1) = a(i)^2 + i */
        cur = cur * cur + (seq_t)i;
    }
    return cur;
}

/*
 * A218740  a(n) = (37^n - 1)/36.
 * https://oeis.org/A218740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (37^n - 1)/36
 * Recurrence: a(0) = 0, a(1) = 1,
 *              a(n) = 38*a(n-1) - 37*a(n-2)  (for n >= 2)
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    seq_t prev = 0;   /* a(0) */
    seq_t cur  = 1;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = 38 * cur - 37 * prev;   /* a(i) */
        prev = cur;
        cur = next;
    }
    return cur;
}

/*
 * A202299  y-values in the solution to x^2 - 18*y^2 = 1.
 * https://oeis.org/A202299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 34*a(n-1) - a(n-2)
 * with initial values a(1) = 0, a(2) = 4.
 */
seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 4;

    seq_t prev = 0;   /* a(1) */
    seq_t cur  = 4;   /* a(2) */
    idx_t i;

    for (i = 3; i <= n; ++i) {
        seq_t next = (seq_t)34 * cur - prev;
        prev = cur;
        cur = next;
    }
    return cur;
}

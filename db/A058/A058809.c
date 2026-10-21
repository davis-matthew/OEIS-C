/*
 * A058809  The sequence lambda(3,n), where lambda is defined in A055203.
 *          Number of ways of placing n identifiable positive intervals with a
 *          total of exactly three starting and/or finishing points.
 * https://oeis.org/A058809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = a(1) = 0; for n >= 2, a(n) = 3 * a(n-1) + 6 (equivalent
 *            to a(n) = 3^n - 3)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t prev = 0;            /* a(1) = 0 */
    for (idx_t i = 2; i <= n; ++i)
        prev = 3 * prev + 6;   /* compute a(i) from a(i-1) */
    return prev;               /* a(n) */
}

/*
 * A030450  Related to number of elements in the free band (idempotent
 *          semigroup) on n generators.
 * https://oeis.org/A030450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; a(n) = (n * a(n-1))^2 for n > 0 */
seq_t a(idx_t n)
{
    seq_t a_cur = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t tmp = (seq_t)i * a_cur;
        a_cur = tmp * tmp;
    }
    return a_cur;
}

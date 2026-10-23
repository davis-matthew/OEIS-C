/*
 * A145506  a(n+1) = a(n)^2 + 2*a(n) - 2 and a(1) = 6.
 * https://oeis.org/A145506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 6, a(n+1) = a(n)^2 + 2*a(n) - 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n <= 1, return the initial term. */
    if (n <= 1)
        return (seq_t)6;

    seq_t current = (seq_t)6;
    for (idx_t i = 1; i < n; ++i) {
        /* Compute the next term from the current one. */
        seq_t prev = current;
        current = prev * prev + 2 * prev - (seq_t)2;
    }
    return current;
}

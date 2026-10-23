/*
 * A156771  a(n) = 729*n - 531.
 * https://oeis.org/A156771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 729*n - 531
 * Range:     u8 n <= 1, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset is 1; for n < 1 the value is undefined, but we return 0. */
    if (n < 1)
        return (seq_t)0;
    return (seq_t)729 * (seq_t)n - (seq_t)531;
}

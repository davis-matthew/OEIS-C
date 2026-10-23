/*
 * A157668  a(n) = 729*n^2 - 442*n + 67.
 * https://oeis.org/A157668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 729*n^2 - 442*n + 67
 * Range:     u8 none, u16 n <= 9, u32 n <= 2427, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The index n is unsigned, so for n < 1 the behaviour is irrelevant. */
    seq_t tn = (seq_t)n;
    return 729 * tn * tn - 442 * tn + 67;
}

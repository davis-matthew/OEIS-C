/*
 * A070430  a(n) = n^2 mod 5.
 * https://oeis.org/A070430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 5
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n mod 5, then square and take mod 5 again. */
    idx_t r = n % 5;          /* r is 0…4 */
    seq_t sr = (seq_t)r;
    return (sr * sr) % 5;     /* seq_t % 5 is defined for the available types */
}

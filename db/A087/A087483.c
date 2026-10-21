/*
 * A087483  Row 0 of the order array of 3/2, i.e., row 0 of the transposable
 *          dispersion in A087465.
 * https://oeis.org/A087483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor((n+1)^2/3)
 * Range:     u8 n <= 26, u16 n <= 442, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)(n + 1);          /* cast to seq_t before multiplications */
    return (x * x) / 3 + 1;            /* integer division floors the result    */
}

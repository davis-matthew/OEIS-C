/*
 * A211534  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w = 3x + 3y.
 * https://oeis.org/A211534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/3) * (floor(n/3) - 1) / 2
 * Range:     u8 n <= 71, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n / 3ull;          /* floor(n/3) */
    if (m < 2)                   /* values for n<6 are zero */
        return 0;

    seq_t mm = (seq_t)m;
    return mm * (mm - 1) / 2u;   /* exact computation in seq_t */
}

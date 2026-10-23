/*
 * A130748  Place n points on each of the three sides of a triangle, 3n points
 *          in all; a(n) = number of nondegenerate triangles that can be
 *          constructed using these points (plus the 3 original vertices) as
 *          vertices.
 * https://oeis.org/A130748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^3 + 15*n^2 + 9*n + 2)/2
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long nll = n;
    unsigned long long val =
        (8ULL * nll * nll * nll
         + 15ULL * nll * nll
         + 9ULL * nll
         + 2ULL) / 2ULL;
    return (seq_t)val;
}

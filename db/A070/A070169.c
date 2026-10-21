/*
 * A070169  Rounded total surface area of a regular tetrahedron with edge
 *          length n.
 * https://oeis.org/A070169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = round(n^2 * sqrt(3))
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Approximation of sqrt(3) as a rational fraction P/Q.
   Here P/Q ≈ 1.732050808, with an absolute error < 1e-9, good enough
   for n ≤ 10,000 (since n^2 ≤ 1e8, the rounding error must be < 5e-9). */
#define SQRT3_NUM 1732050808ULL   /* numerator */
#define SQRT3_DEN 1000000000ULL   /* denominator (even) */

seq_t a(idx_t n)
{
    /* Compute n^2 * P + Q/2, then divide by Q.
       All arithmetic is done with seq_t; intermediate fits
       within seq_t for all supported builds. */
    seq_t nn = (seq_t)n * (seq_t)n;              /* n^2 */
    seq_t numer = nn * (seq_t)SQRT3_NUM + (SQRT3_DEN >> 1); /* n^2*P + Q/2 */
    return numer / (seq_t)SQRT3_DEN;             /* floor division gives round */
}

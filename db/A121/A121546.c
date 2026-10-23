/*
 * A121546  a(n) = dimension of the space in which the sphere of radius n is
 *          of maximum volume.
 * https://oeis.org/A121546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2π*n^2 - 0.5)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer approximation: π ≈ 6283185307 / 2000000000 */

static const seq_t _P = (seq_t)6283185307ULL;      /* numerator   */
static const seq_t _D = (seq_t)2000000000ULL;     /* denominator */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n * (seq_t)n;           /* n^2      */
    seq_t k    = ((seq_t)2 * _P) * nn;          /* 2P * n^2 */
    seq_t num  = k - (_D / 2);                  /* subtract 0.5 */
    return num / _D;                            /* divide by D */
}

/*
 * A008579  Coordination sequence for planar net 3.6.3.6. Spherical growth
 *          function for a certain reflection group in plane.
 * https://oeis.org/A008579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(0)=1, a(1)=4; for n>1 * a(n) = ((9+(-1)^n)*n)/2 - 2*(-1)^n
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;

    /* n>1 */
    if ((n & 1) == 0) {   /* even n: (-1)^n = 1 */
        /* a(n) = (10*n)/2 - 2 */
        return (((seq_t)10 * n) / 2) - 2;
    } else {              /* odd n: (-1)^n = -1 */
        /* a(n) = (8*n)/2 + 2 */
        return (((seq_t)8 * n) / 2) + 2;
    }
}

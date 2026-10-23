/*
 * A195034  Vertex number of a square spiral in which the length of the first
 *          two edges are the legs of the primitive Pythagorean triple [21,
 *          20, 29]. The edges of the spiral have length A195033.
 * https://oeis.org/A195034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(41*n+83) - (2*n+43)*(-1)^n + 43) / 16
 * Range:     u8 n <= 0, u16 n <= 27, u32 n <= 7435, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently) a(0)=0, a(1)=21, a(2)=41, a(3)=83 … */

seq_t a(idx_t n)
{
    /* Convert index to the result type */
    seq_t sn = (seq_t)n;

    /* Compute 2*n*(41*n+83) */
    seq_t part1 = sn * ( (seq_t)41 * sn + (seq_t)83 );
    part1 = part1 * (seq_t)2;

    /* Compute (2*n+43) */
    seq_t part2 = (seq_t)2 * sn + (seq_t)43;

    seq_t numerator;
    if ((n & (idx_t)1) == 0) {           /* even n : subtract part2       */
        numerator = part1 - part2 + (seq_t)43;
    } else {                              /* odd n : add part2             */
        numerator = part1 + part2 + (seq_t)43;
    }

    return numerator / (seq_t)16;
}

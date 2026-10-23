/*
 * A118822  Numerators of the convergents of the 2-adic continued fraction of
 *          zero given by A118821.
 * https://oeis.org/A118822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, sign, easy
 * Formula:   a(n) = sqrt(((n+1)^2)%8) * (-1)^(floor((n+2)/4)) Equivalent
 *            periodic sequence with period 8: [2,-1,0,-1,-2,1,0,1]
 * Range:     i8 n <= 108, i16 n <= 108, i32 n <= 108, i64 n <= 108,
 *            i128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[8] = {2, -1, 0, -1, -2, 1, 0, 1};
    /* n is guaranteed >= 1 */
    return period[(n - 1) % 8];
}

/*
 * A118719  Cubes for which the digital root is also a cube.
 * https://oeis.org/A118719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1) = 0, for n ≥ 2, a(n) = (floor(3*n/2) - 2)^3
 * Range:     u8 n <= 5, u16 n <= 28, u32 n <= 1085, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)0;
    }

    /* Compute floor(3*n/2) *without* using floating point */
    seq_t t = (seq_t)n;
    seq_t m = (t * (seq_t)3) / (seq_t)2;   /* floor division */
    m = m - (seq_t)2;                      /* subtract 2 */

    /* cube the result (no overflow detection on purpose; 
       correct result if it fits in seq_t) */
    seq_t cube = m * m * m;

    return cube;
}

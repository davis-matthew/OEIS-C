/*
 * A195032  Vertex number of a square spiral in which the length of the first
 *          two edges are the legs of the primitive Pythagorean triple [5, 12,
 *          13]. The edges of the spiral have length A195031.
 * https://oeis.org/A195032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(17*n + 27) + (14*n - 3)*(-1)^n + 3) / 16
 * Range:     u8 n <= 1, u16 n <= 43, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t ns = (seq_t)n;                /* cast index to seq_t */
    seq_t t1 = 17 * ns + 27;            /* 17*n + 27 */
    seq_t t2 = 2 * ns * t1;             /* 2*n*(17*n + 27) */
    seq_t total = t2 + 3;               /* add the constant 3 */
    seq_t sub   = 14 * ns - 3;          /* 14*n - 3 */
    if ((n & 1) == 0) {
        total = total + sub;            /* even n : add (14*n-3) */
    } else {
        total = total - sub;            /* odd n  : subtract (14*n-3) */
    }
    return total / 16;
}

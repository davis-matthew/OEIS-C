/*
 * A066456  Upper bound on number of regular triangulations of cyclic polytope
 *          C(n, n-4).
 * https://oeis.org/A066456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - 8*n^3 + 52*n^2 - 112*n + 128)/64 if n is even; *
 *            a(n) = (n^4 - 8*n^3 + 54*n^2 - 120*n + 137)/64 if n is odd
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 432, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* values for n < 1 are undefined, but avoid UB */

    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;              /* n^2 */
    seq_t N3 = N2 * N;             /* n^3 */
    seq_t N4 = N3 * N;             /* n^4 */

    seq_t num;
    if ((n & 1) == 0) {            /* even n */
        num = N4
              - (seq_t)8 * N3
              + (seq_t)52 * N2
              - (seq_t)112 * N
              + (seq_t)128;
    } else {                       /* odd n */
        num = N4
              - (seq_t)8 * N3
              + (seq_t)54 * N2
              - (seq_t)120 * N
              + (seq_t)137;
    }

    return num / (seq_t)64;
}

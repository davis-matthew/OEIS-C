/*
 * A127877  Integers of the form (x^4)/24 + (x^3)/6 + (x^2)/2 + x + 1 with x >
 *          0.
 * https://oeis.org/A127877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11 +5*(-1)^n +16*(2+(-1)^n)*n +18*(3+(-1)^n)*n^2
 *            +36*(1+(-1)^n)*n^3 +54*n^4)/16 Equivalent piecewise expressions
 *            for speed: a(n) = (27*n^4 + 36*n^3 + 36*n^2 + 24*n + 8)/8 for n
 *            even a(n) = (27*n^4 + 18*n^2 + 8*n + 3)/8 for n odd
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 112, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n must be >= 1 as per OEIS offset */
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;        /* n^2 */
    seq_t n3 = n2 * N;       /* n^3 */
    seq_t n4 = n2 * n2;      /* n^4 */

    if ((n & 1) == 0) {      /* even n */
        seq_t num = (seq_t)27 * n4
                  + (seq_t)36 * n3
                  + (seq_t)36 * n2
                  + (seq_t)24 * N
                  + (seq_t)8;
        return num / 8;      /* divide by 8 */
    } else {                 /* odd n */
        seq_t num = (seq_t)27 * n4
                  + (seq_t)18 * n2
                  + (seq_t)8  * N
                  + (seq_t)3;
        return num / 8;      /* divide by 8 */
    }
}

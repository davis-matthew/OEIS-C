/*
 * A193400  Hyper-Wiener index of a benzenoid consisting of a chain of n
 *          hexagons characterized by the encoding s = 1133 (see the Gutman et
 *          al. reference, Sec. 5).
 * https://oeis.org/A193400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( 6*n^4 +40*n^3 +114*n^2 +16*n -45 +(-1)^n*(6*n^2 +20*n
 *            -63) )/4
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 228, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;        /* n^2 */
    seq_t n3 = n2 * nn;        /* n^3 */
    seq_t n4 = n3 * nn;        /* n^4 */

    seq_t first  = 6 * n4 + 40 * n3 + 114 * n2 + 16 * nn - 45; /* 6n^4+40n^3+114n^2+16n-45 */
    seq_t second = 6 * n2 + 20 * nn - 63;                     /* 6n^2+20n-63 */

    /* (-1)^n : +1 for even n, -1 for odd n */
    if ((n & 1) == 0) {
        first += second;
    } else {
        first -= second;
    }

    return first / 4;
}

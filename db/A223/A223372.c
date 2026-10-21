/*
 * A223372  3X3 square grid graph coloring a rectangular array: number of nX1
 *          0..8 arrays where 0..8 label nodes of the square grid graph and
 *          every array movement to a horizontal or vertical neighbor moves
 *          along an edge of this graph.
 * https://oeis.org/A223372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-2) for n>3; with a(1)=9, a(2)=24, a(3)=68
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 41, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 9;
    if (n == 2) return 24;
    if (n == 3) return 68;

    if ((n & 1) == 0) {  /* even n */
        idx_t k = n / 2 - 1;          /* number of times to multiply by 8 */
        seq_t res = 24;
        for (idx_t i = 0; i < k; ++i)
            res *= 8;
        return res;
    } else {            /* odd n (n > 3) */
        idx_t k = (n - 3) / 2;          /* number of times to multiply by 8 */
        seq_t res = 68;
        for (idx_t i = 0; i < k; ++i)
            res *= 8;
        return res;
    }
}

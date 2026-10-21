/*
 * A208140  Number of n X 6 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A208140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n^6 + 24*n^5 + 52*n^4 + 48*n^3 + 16*n^2)/9 (equivalent
 *            to the OEIS expression (4/9)n^6 + (8/3)n^5 + (52/9)n^4 +
 *            (16/3)n^3 + (16/9)n^2)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 44, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt   = (seq_t)n;          /* n as seq_t */
    seq_t n2   = nt * nt;           /* n^2 */
    seq_t n3   = n2 * nt;           /* n^3 */
    seq_t n4   = n2 * n2;           /* n^4 */
    seq_t n5   = n4 * nt;           /* n^5 */
    seq_t n6   = n4 * n2;           /* n^6 */

    /* numerator = 4*n^6 + 24*n^5 + 52*n^4 + 48*n^3 + 16*n^2 */
    seq_t numerator = (seq_t)4 * n6
                     + (seq_t)24 * n5
                     + (seq_t)52 * n4
                     + (seq_t)48 * n3
                     + (seq_t)16 * n2;

    return numerator / (seq_t)9;    /* division is exact for all integer n */
}

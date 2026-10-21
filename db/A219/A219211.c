/*
 * A219211  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical, diagonal or antidiagonal
 *          neighbors in a random, but sorted with lexicographically
 *          nondecreasing rows and columns, 0..2 n X 2 array.
 * https://oeis.org/A219211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 - 10 n^3 + 59 n^2 - 74 n + 24) / 24 for n >= 4; a(1)
 *            = 3, a(2) = 3, a(3) = 7
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 185, u64 n <= 185,
 *            u128 n <= 185, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small n special values */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)7;

    seq_t nv = (seq_t)n;
    seq_t n2 = nv * nv;        /* n^2 */
    seq_t n3 = n2 * nv;        /* n^3 */
    seq_t n4 = n3 * nv;        /* n^4 */

    seq_t numer = n4
                  - (seq_t)10 * n3
                  + (seq_t)59 * n2
                  - (seq_t)74 * nv
                  + (seq_t)24;

    return numer / (seq_t)24;
}

/*
 * A220147  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal or diagonal neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..2 n X 2 array.
 * https://oeis.org/A220147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 - 2*n^3 + 29*n^2 + 8*n)/12 , n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t once to avoid repeated casts */
    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;          /* n^2 */
    seq_t n3   = n2 * nn;          /* n^3 */
    seq_t n4   = n3 * nn;          /* n^4 */

    seq_t numer = n4
                - (seq_t)2 * n3
                + (seq_t)29 * n2
                + (seq_t)8 * nn;

    return numer / (seq_t)12;
}

/*
 * A138849  a(n) = AlexanderPolynomial[n] defined as Det[Transpose[S]-n S]
 *          where S is Kronecker Product of two 2 X 2 Seifert matrices {{-1,
 *          1}, {0, -1}} [X] {{-1, 1}, {0, -1}} = {{1, -1, -1, 1}, {0, 1, 0,
 *          -1}, {0, 0, 1, -1}, {0, 0, 0, 1}}.
 * https://oeis.org/A138849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - n + 1)*(n-2)^2
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert index to the sequence type once */
    seq_t N = (seq_t)n;
    /* Compute n^2 */
    seq_t n2 = N * N;
    /* n^2 - n + 1 */
    seq_t part1 = n2 - N + (seq_t)1;
    /* (n-2)^2 = n^2 - 4n + 4 */
    seq_t part2 = n2 - (N * (seq_t)4) + (seq_t)4;
    return part1 * part2;
}

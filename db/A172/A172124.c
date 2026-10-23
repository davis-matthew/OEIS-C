/*
 * A172124  Number of ways to place 3 nonattacking bishops on an n X n board.
 * https://oeis.org/A172124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-2)*(2*n^4 - 4*n^3 + 7*n^2 - 6*n + 4)/12 for even n;
 *            * a(n) = (n-1)*(2*n^5 - 6*n^4 + 9*n^3 - 11*n^2 + 5*n - 3)/12 for
 *            odd n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 53, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Cast the index to the sequence type once. */
    seq_t N = (seq_t)n;
    
    /* Pre‑compute powers of N.  These can be large, but are handled
       correctly by the arbitrary‑precision type. */
    seq_t N2 = N * N;          /* N^2 */
    seq_t N3 = N2 * N;         /* N^3 */
    seq_t N4 = N3 * N;         /* N^4 */
    seq_t N5 = N4 * N;         /* N^5 */
    
    if ((n & 1ULL) == 0) {    /* even n */
        /* Compute the polynomial 2*N^4 - 4*N^3 + 7*N^2 - 6*N + 4. */
        seq_t poly = (seq_t)2 * N4
                   - (seq_t)4 * N3
                   + (seq_t)7 * N2
                   - (seq_t)6 * N
                   + (seq_t)4;
        /* Multiply by N*(N-2) and divide by 12. */
        seq_t prod = N * (N - (seq_t)2);
        prod = prod * poly;
        return prod / (seq_t)12;
    } else {                  /* odd n */
        /* Compute the polynomial 2*N^5 - 6*N^4 + 9*N^3 - 11*N^2 + 5*N - 3. */
        seq_t poly = (seq_t)2 * N5
                   - (seq_t)6 * N4
                   + (seq_t)9 * N3
                   - (seq_t)11 * N2
                   + (seq_t)5 * N
                   - (seq_t)3;
        /* Multiply by (N-1) and divide by 12. */
        seq_t prod = (N - (seq_t)1) * poly;
        return prod / (seq_t)12;
    }
}

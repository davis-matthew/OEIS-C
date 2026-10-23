/*
 * A011826  f-vectors for simplicial complexes of dimension at most 1 (graphs)
 *          on at most n-1 vertices.
 * https://oeis.org/A011826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A011826: Number of f-vectors for simplicial complexes
 * of dimension at most 1 (graphs) on at most n-1 vertices.
 * Formula: a(n) = (n^3 - 3*n^2 + 8*n + 6) / 6
 *
 * a(1) = 2
 * a(2) = 3
 * a(3) = 5
 * ...
 */

seq_t a(idx_t n) {
    /* Convert index to seq_t for correct arithmetic */
    seq_t nn = (seq_t)n;

    /* Compute the numerator of the formula */
    seq_t num = nn * nn * nn          /* n^3          */
             - (seq_t)3 * nn * nn    /* 3*n^2        */
             + (seq_t)8 * nn         /* 8*n          */
             + (seq_t)6;             /* +6           */

    /* Divide by 6 - guaranteed to be integer */
    return num / (seq_t)6;
}

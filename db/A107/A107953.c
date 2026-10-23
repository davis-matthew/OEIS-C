/*
 * A107953  Number of chains in the power set lattice of an (n+3)-element set
 *          X_(n+3) of specification n^1 2^1 1, that is, n identical objects
 *          of one kind, 2 identical objects of another kind and one other
 *          kind. It is the same as the number of fuzzy subsets X_(n+3).
 * https://oeis.org/A107953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   a(n) = 2^(n+1) * ((n^2/2)*(n+13) + 21*n + 16) - 1
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 18, u64 n <= 46, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute the polynomial part: (n^2*(n+13))/2 + 21*n + 16 */
    seq_t nn = (seq_t)n;
    seq_t expr = (nn * nn * (nn + (seq_t)13)) / (seq_t)2;
    expr += (seq_t)21 * nn;
    expr += (seq_t)16;

    /* Compute 2^(n+1) by repeated multiplication by 2 */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n + 1; ++i) {
        pow2 *= (seq_t)2;
    }

    /* Final result */
    seq_t result = pow2 * expr;
    result -= (seq_t)1;
    return result;
}

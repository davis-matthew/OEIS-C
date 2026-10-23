/*
 * A161697  Number of reduced words of length n in the Weyl group B_4.
 * https://oeis.org/A161697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in
 *            (1+x)(1+x+x^2+x^3)(1+x+...+x^5)(1+x+...+x^7)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of quadruples (i,j,k,l) with i∈[0,1], j∈[0,3], k∈[0,5], l∈[0,7] and i+j+k+l = n. */

/* Return type seq_t and index type idx_t are defined by the build. */
seq_t a(idx_t n)
{
    /* The support of the sequence is 0..16.  For n > 16 the value is 0. */
    if (n > 16) return (seq_t)0;

    seq_t ans = 0;
    /* Iterate over all possible values of the four summands. */
    for (idx_t i = 0; i <= 1; ++i) {
        for (idx_t j = 0; j <= 3; ++j) {
            for (idx_t k = 0; k <= 5; ++k) {
                /* l is determined by the desired sum. Compute it directly. */
                idx_t l = n - (i + j + k);
                if (l <= 7 && l >= 0) {
                    /* All bounds satisfied; this quadruple contributes 1. */
                    ans = ans + (seq_t)1;
                }
            }
        }
    }
    return ans;
}

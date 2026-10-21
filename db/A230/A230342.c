/*
 * A230342  Number of permutations of [2n+2] in which the longest increasing
 *          run has length n+2.
 * https://oeis.org/A230342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3+6*n^2+12*n+11)*(2*n+2)!/(n+4)! for n>0, with a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A230342 */
seq_t a(idx_t n)
{
    /* Handle offset 0 explicitly */
    if (n == 0)
        return (seq_t)1;

    seq_t n_val = n;
    /* Polynomial part: n^3 + 6 n^2 + 12 n + 11 */
    seq_t poly = n_val * n_val * n_val
                 + 6 * n_val * n_val
                 + 12 * n_val
                 + (seq_t)11;

    /* Product part: (2n+2)!/(n+4)!.
       For n >= 3 this is ∏_{k=n+5}^{2n+2} k.
       For n = 1 the ratio is 1/(n+4) = 1/5.
       For n = 2 the ratio is 1 (empty product). */
    if (n >= 3) {
        for (idx_t k = n + 5; k <= 2 * n + 2; ++k) {
            poly *= (seq_t)k;
        }
    } else if (n == 1) {
        poly /= (seq_t)(n + 4);    /* divide by 5 */
    }

    return poly;
}

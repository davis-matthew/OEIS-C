/*
 * A116701  Number of permutations of length n that avoid the patterns 132,
 *          4321.
 * https://oeis.org/A116701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1,4) + binomial(n,4) + binomial(n,2) + 1
 * Range:     u8 n <= 8, u16 n <= 30, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1; /* +1 term */

    /* binomial(n,2) = n*(n-1)/2 */
    if (n >= 2) {
        seq_t t = (seq_t)n * ((seq_t)n - 1) / 2;
        result += t;
    }

    /* binomial(n,4) = n*(n-1)*(n-2)*(n-3)/24 */
    if (n >= 4) {
        seq_t t = (seq_t)n * ((seq_t)n - 1) * ((seq_t)n - 2) * ((seq_t)n - 3) / 24;
        result += t;
    }

    /* binomial(n+1,4) = (n+1)*n*(n-1)*(n-2)/24   (valid for n >= 3) */
    if (n + 1 >= 4) {
        seq_t t = ((seq_t)(n + 1)) * (seq_t)n * ((seq_t)(n - 1)) * ((seq_t)(n - 2)) / 24;
        result += t;
    }

    return result;
}

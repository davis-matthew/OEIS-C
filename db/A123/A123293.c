/*
 * A123293  Number of permutations of n distinct letters (ABCD...) each of
 *          which appears 4 times and having n-3 fixed points.
 * https://oeis.org/A123293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 64 * n * (n-1) * (n+1) / 3
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* For n = 0 or 1 the result is 0 (avoids underflow in n-1) */
    if (n < 2) {
        return 0;
    }
    seq_t n_t = (seq_t)n;
    seq_t prod = n_t * (n_t - 1) * (n_t + 1);  /* n*(n-1)*(n+1) */
    prod = prod * 64;                          /* multiply by 64 */
    prod /= 3;                                 /* divide by 3 (exact division) */
    return prod;
}

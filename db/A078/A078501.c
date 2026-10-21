/*
 * A078501  a(n) = Sum_{k=1..n^2} A078446(k).
 * https://oeis.org/A078501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n-2)*2^n + n*(n+1)/2 + 4
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the offset‑1 trivial cases explicitly to avoid negative
     * intermediate when n = 1.  For n ≥ 2 the expression is
     * non‑negative, so we can safely work with unsigned seq_t. */
    if (n == 0) {
        return 0;           /* undefined by OEIS, but harmless */
    }
    if (n == 1) {
        return 1;           /* a(1) = 1 */
    }

    /* Compute 2^n as a seq_t */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 <<= 1;         /* multiply by 2 */
    }

    /* Compute term1 = 2 * (n-2) * 2^n */
    seq_t term1 = 2 * ((seq_t)(n - 2)) * pow2;

    /* Compute term2 = n*(n+1)/2 (triangular number) */
    seq_t term2 = ((seq_t)n * ((seq_t)n + 1)) / 2;

    /* Sum all parts and add 4 */
    return term1 + term2 + 4;
}

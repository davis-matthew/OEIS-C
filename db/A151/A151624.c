/*
 * A151624  Number of permutations of 2 indistinguishable copies of 1..n with
 *          exactly 2 adjacent element pairs in decreasing order.
 * https://oeis.org/A151624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - (2*n + 1)*3^n + n*(2*n + 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* two_n_plus1 = 2*n + 1 */
    seq_t two_n_plus1 = ((seq_t)2 * (seq_t)n) + (seq_t)1;

    /* compute 3^n and 6^n simultaneously using exponentiation by squaring */
    seq_t pow3   = (seq_t)1;
    seq_t pow6   = (seq_t)1;
    seq_t base3  = (seq_t)3;
    seq_t base6  = (seq_t)6;
    int exp = (int)n;                /* n <= 10000 fits in int */

    while (exp > 0) {
        if (exp & 1) {
            pow3 = pow3 * base3;     /* multiply by current power of 3 */
            pow6 = pow6 * base6;     /* multiply by current power of 6 */
        }
        base3 = base3 * base3;       /* square for next bit */
        base6 = base6 * base6;
        exp >>= 1;
    }

    /* n*(2*n + 1) */
    seq_t term3 = (seq_t)n * two_n_plus1;

    /* a(n) = 6^n + n*(2*n + 1) - (2*n + 1)*3^n */
    seq_t result = pow6 + term3 - two_n_plus1 * pow3;
    return result;
}

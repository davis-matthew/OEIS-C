/*
 * A007762  Number of domino tilings of a certain region.
 * https://oeis.org/A007762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, unkn, more
 * Formula:   a(n) = A006318(n-1) * A104550(n), where A006318(m): (m+1)*S(m) =
 *            3*(2m-1)*S(m-1) - (m-2)*S(m-2), S(0) = 1, S(1) = 2; A104550(n) =
 *            Sum_{k=0..n-1} binomial(n-1+k, 2k) * binomial(2k+1, k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t b104550(idx_t n) {
    idx_t m = n - 1;
    seq_t t = 1;   /* binomial(m+k, 2k) * binomial(2k+1, k) at k = 0 */
    seq_t sum = 1;
    for (idx_t k = 0; k < m; k++) {
        /* binomial(m+k,2k) -> binomial(m+k+1,2k+2) */
        t = t * (seq_t)(m + k + 1) * (seq_t)(m - k) / ((seq_t)(2 * k + 1) * (seq_t)(2 * k + 2));
        /* binomial(2k+1,k) -> binomial(2k+3,k+1) */
        t = t * (seq_t)(2 * (2 * k + 3)) / (seq_t)(k + 2);
        sum += t;
    }
    return sum;
}

static seq_t schroeder(idx_t m) {
    if (m == 0) return 1;
    if (m == 1) return 2;
    seq_t prev2 = 1, prev1 = 2;
    for (idx_t i = 2; i <= m; i++) {
        seq_t curr = ((seq_t)(6 * i - 3) * prev1 - (seq_t)(i - 2) * prev2) / (seq_t)(i + 1);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

seq_t a(idx_t n) {
    return schroeder(n - 1) * b104550(n);
}

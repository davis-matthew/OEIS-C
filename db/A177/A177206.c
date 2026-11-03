/*
 * A177206  a(n) = 2*binomial(n+4, 4) + n + 4.
 * https://oeis.org/A177206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*binomial(n+4, 4) + n + 4
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 37 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom_n_plus4_choose4(idx_t n) {
    seq_t res = 1;
    seq_t nn = (seq_t)n;
    for (idx_t i = 1; i <= 4; ++i) {
        seq_t numerator = nn + i;   /* n + i  (i = 1..4) */
        res = res * numerator;
        res = res / i;              /* exact division */
    }
    return res;                     /* binomial(n+4, 4) */
}
/* Computed by evaluating the binomial coefficient via an iterative
   division that keeps all intermediate results integral. */

seq_t a(idx_t n) {
    seq_t bin4 = binom_n_plus4_choose4(n);
    seq_t result = bin4 * 2 + (seq_t)n + 4;
    return result;
}

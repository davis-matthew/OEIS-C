/*
 * A051923  Partial sums of A051836.
 * https://oeis.org/A051923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5,5)*(n+2)/2
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 92, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 51 terms through n = 872 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom5(idx_t N) {
    /* Compute C(N,5) for N >= 5 */
    seq_t res = (seq_t)1;
    /* Multiplicative formula: res = res * (N-i+1)/i for i=1..5 */
    for (idx_t i = 1; i <= 5; ++i) {
        res = res * ((seq_t)(N - i + 1));
        res = res / (seq_t)i;  /* exact division */
    }
    return res;
}

seq_t a(idx_t n) {
    /* a(n) = binomial(n+5,5) * (n+2) / 2 */
    seq_t bin = binom5(n + 5);
    seq_t result = bin * ((seq_t)n + 2);
    result = result / 2;
    return result;
}

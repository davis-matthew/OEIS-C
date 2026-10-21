/*
 * A052780  Expansion of e.g.f. x^2*exp(4*x).
 * https://oeis.org/A052780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n-1)*4^(n-2) for n >= 2, a(0) = 0, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) return 0;

    /* Compute 4^(n-2) iteratively */
    seq_t pow = 1;
    for (idx_t i = 0; i < n - 2; ++i) {
        pow *= (seq_t)4;
    }

    /* Final result: n * (n-1) * 4^(n-2) */
    seq_t result = (seq_t)n * (seq_t)(n - 1) * pow;
    return result;
}

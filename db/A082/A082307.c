/*
 * A082307  Expansion of e.g.f. (1+x)*exp(3*x)*cosh(x).
 * https://oeis.org/A082307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   For n == 0: a(0) = 1. For n >= 1: a(n) = ((n+2) * 2^(n-1) +
 *            (n+4) * 4^(n-1)) / 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }

    /* compute 2^(n-1) */
    seq_t pow2 = 1;
    for (idx_t i = 1; i < n; ++i) {
        pow2 *= 2;
    }

    /* compute 4^(n-1) = (2^(n-1))^2 */
    seq_t pow4 = pow2 * pow2;

    seq_t part1 = (seq_t)(n + 2) * pow2;
    seq_t part2 = (seq_t)(n + 4) * pow4;
    seq_t sum   = part1 + part2;

    return sum / 2;
}

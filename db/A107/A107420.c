/*
 * A107420  a(n) = binomial(n+5,5)*binomial(n+8,8).
 * https://oeis.org/A107420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+5,5) * binomial(n+8,8)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 14, u64 n <= 95, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A107420 = C(n+5,5) * C(n+8,8) for n >= 0 */

static inline seq_t binom_small(idx_t n, idx_t k) {
    /* K is very small (<=8) so we do the multiplicative formula safely. */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Use the standard iterative form: res *= (n - k + i); res /= i; */
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

seq_t a(idx_t n) {
    /* A107420 = C(n+5,5) * C(n+8,8) */
    return binom_small(n + 5, 5) * binom_small(n + 8, 8);
}

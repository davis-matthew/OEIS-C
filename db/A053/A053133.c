/*
 * A053133  One half of binomial coefficients binomial(2*n-8,9).
 * https://oeis.org/A053133
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-8, 9)/2, with offset 9 (a(9)=5)
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 30, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Validated: standard, pass, 43 terms through n = 198 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_9(idx_t N)
{
    /* Compute C(N,9) multiplicatively, using a wider type for safety. */
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= 9; ++i) {
        res = res * (N - 9 + i) / i;
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    if (n < 9) return 0;
    idx_t N = 2 * n - 8;
    seq_t val = binom_9(N);
    return val / 2;
}

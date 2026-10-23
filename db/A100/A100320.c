/*
 * A100320  A Catalan transform of (1 + 2*x)/(1 - 2*x).
 * https://oeis.org/A100320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4 * binomial(2*n-1, n), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute the binomial coefficient iteratively:
           res = res * (n-1+i) / i;  division is exact. */
        res = res * ((seq_t)(n - 1 + i)) / (seq_t)i;
    }
    return res * (seq_t)4;
}

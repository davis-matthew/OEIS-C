/*
 * A158363  a(n) = binomial((n+2)^3,n+1).
 * https://oeis.org/A158363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial((n+2)^3, n+1)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n + 1;
    idx_t n2 = n + 2;
    idx_t m = n2 * n2 * n2;               /* (n+2)^3 */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t numer = (seq_t)(m - k + i); /* (m - k + i) fits in idx_t */
        result = result * numer / i;     /* integer division is exact */
    }
    return result;
}

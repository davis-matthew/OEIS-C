/*
 * A175673  Number of n-digit terms in A179955.
 * https://oeis.org/A175673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) = 0 if n = 1 * a(n) = C(9, n-1) if 2 <= n <= 10 * a(n) = 0
 *            if n > 10 * where C(9, k) is the binomial coefficient 9 choose k
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)               /* no 1‑digit terms in A179955 */
        return 0;
    idx_t k = n - 1;
    if (k > 9)                /* C(9, k) = 0 for k > 9 */
        return 0;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Use the multiplicative formula for binomial coefficients:
           C(9, k) = Product_{i=1..k} (9 - k + i) / i */
        res = res * (seq_t)(9 - k + i) / (seq_t)i;
    }
    return res;
}

/*
 * A050214  a(n) = Product_{d|n, d^2<=n} (d+n/d); a(1)=1.
 * https://oeis.org/A050214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{d|n, d^2≤n, d<n} (d + n/d)
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for 0, return 0 */
    seq_t prod = (seq_t)1;
    /* Iterate over divisors d with d*d ≤ n (avoid overflow via division) */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;      /* d is a divisor */
        if (d == n) continue;          /* exclude d = n (square case d==n only when n=1) */
        seq_t term = (seq_t)d + (seq_t)(n / d);
        prod *= term;
    }
    return prod;
}

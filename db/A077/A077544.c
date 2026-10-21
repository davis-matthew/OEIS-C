/*
 * A077544  Product of next n numbers + sum of next n numbers.
 * https://oeis.org/A077544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (k+1)(k+2)...(k+n) + n*k + n*(n+1)/2 where k = n*(n-1)/2
 *            (product of the next n integers plus their sum)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n * (seq_t)(n - 1) / 2;      /* compute k as seq_t */
    seq_t prod = 1;                               /* product of k+1 … k+n */

    for (idx_t i = 1; i <= n; ++i)
        prod *= (k + i);

    seq_t sum = (seq_t)n * k + (seq_t)n * (n + 1) / 2;
    return prod + sum;
}

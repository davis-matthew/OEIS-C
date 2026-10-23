/*
 * A112858  Table read by antidiagonals: T(n,k) = count of increasing runs in
 *          strings of length n*k formed by concatenating k permutations of
 *          [n].
 * https://oeis.org/A112858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r, s-r), where s = smallest integer with (s-1)*s/2 ≥ n
 *            r = n - (s-2)*(s-1)/2 (1 ≤ r < s) Let k = s-r, n = r. Then
 *            T(n,k) = ((k*n*(n+1) - (k-1)*(n-1)) / (2*n)) * (n!)^k
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 32, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find s such that (s-1)*s/2 >= n */
    idx_t s = 2;
    while ((s - 1) * s / 2 < n) ++s;

    /* Compute r and k */
    idx_t prev_total = (s - 2) * (s - 1) / 2;
    idx_t r = n - prev_total;      /* n in T(n,k) */
    idx_t k = s - r;                /* k in T(n,k) */

    /* Compute factorial of r (i.e., n) */
    seq_t fac = 1;
    for (idx_t i = 2; i <= r; ++i)
        fac *= (seq_t)i;

    /* Compute (n!)^k by repeated multiplication */
    seq_t pow_fac = 1;
    for (idx_t j = 0; j < k; ++j)
        pow_fac *= fac;

    /* Compute numerator and denominator of the fraction */
    seq_t num = (seq_t)k * (seq_t)r * (seq_t)(r + 1)
                - (seq_t)(k - 1) * (seq_t)(r - 1);
    seq_t denom = (seq_t)2 * (seq_t)r;

    /* Final result */
    return (num * pow_fac) / denom;
}

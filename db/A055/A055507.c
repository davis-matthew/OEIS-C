/*
 * A055507  a(n) = Sum_{k=1..n} d(k)*d(n+1-k), where d(k) is number of
 *          positive divisors of k.
 * https://oeis.org/A055507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} d(k)*d(n+1-k) where d(k) is number of *
 *            positive divisors of k. This implementation computes the divisor
 *            * counts up to n with an O(n log n) sieve each call and then *
 *            evaluates the convolution
 * Range:     u8 n <= 22, u16 n <= 1006, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset starts at 1 */

    /* Allocate array to hold divisor counts up to n.  The counts
     * are small (≤ n) so uint32_t is sufficient. */
    uint32_t *d = (uint32_t *)malloc((size_t)(n + 1) * sizeof(uint32_t));
    if (!d)
        abort();

    /* initialise counts to zero */
    for (idx_t i = 0; i <= n; ++i)
        d[i] = 0;

    /* sieve for divisor counts */
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            d[j] += 1;

    /* compute the convolution */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
        sum += (seq_t)d[k] * (seq_t)d[n + 1 - k];

    free(d);
    return sum;
}

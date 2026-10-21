/*
 * A099654  a(n) is the number of n-subsets [n=1,2,...,10] of the 10 decimal
 *          digits from which no prime numbers can be constructed. See also
 *          A099653.
 * https://oeis.org/A099654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = binomial(6,n) + binomial(4,n) for n>1, a(1)=5
 * Range:     u8 n <= 131, u16 n <= 131, u32 n <= 131, u64 n <= 131,
 *            u128 n <= 131, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial coefficient for small n */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * ((seq_t)(n - k + i)) / (seq_t)i;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 5;
    return binom_small(6, n) + binom_small(4, n);
}

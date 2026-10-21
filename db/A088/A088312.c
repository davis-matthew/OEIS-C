/*
 * A088312  Number of sets of lists (cf. A000262) with even number of lists.
 * https://oeis.org/A088312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..⌊n/2⌋} n!/(2k)! * C(n-1, 2k-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses only integer arithmetic and the task's */
/* provided types.  It works for all supported seq_t implementations. */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) using a multiplicative approach. */
static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t r = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        r = r * (seq_t)(n - k + i) / (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Handle the first two offsets explicitly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;

    idx_t maxk = n / 2; /* floor(n/2) */
    seq_t res = (seq_t)0;

    /* P(k) = n!/(2k)! for current k.  Start with k = maxk. */
    seq_t prod = (seq_t)1;
    if (2 * maxk < n)          /* compute trailing product if 2k < n */
    {
        for (idx_t t = 2 * maxk + 1; t <= n; ++t)
            prod *= (seq_t)t;
    }

    for (idx_t k = maxk; k >= 1; --k)
    {
        seq_t bin = binom_calc(n - 1, 2 * k - 1);
        res += prod * bin;
        if (k > 1)           /* prepare prod for next k-1 */
        {
            prod *= (seq_t)(2 * k - 1);
            prod *= (seq_t)(2 * k);
        }
    }
    return res;
}

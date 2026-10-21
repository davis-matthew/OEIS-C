/*
 * A226355  Number of ordered pairs (i,j) with |i| * |j| <= n.
 * https://oeis.org/A226355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 4n + 4*Sum_{k=1..n} tau(k)
 * Range:     u8 n <= 15, u16 n <= 1884, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for divisor counts and their prefix sums */
static uint32_t *tau       = NULL;   /* tau[i] = number of divisors of i, 1 <= i <= cap */
static seq_t   *sumTau      = NULL;   /* sumTau[i] = sum_{k=1..i} tau(k) */
static idx_t    cap         = 0;      /* largest index for which arrays are initialized */

/* ensure arrays are allocated and filled up to "up_to" */
static void ensure_capacity(idx_t up_to)
{
    if (up_to <= cap)
        return;                      /* already computed */

    idx_t new_cap = up_to;

    /* reallocate arrays: cast for C++ compatibility */
    uint32_t *tmp_tau   = (uint32_t *)realloc(tau,   (new_cap + 1) * sizeof(uint32_t));
    seq_t    *tmp_sum   = (seq_t    *)realloc(sumTau,(new_cap + 1) * sizeof(seq_t  ));
    if (!tmp_tau || !tmp_sum) abort();

    /* zero the entire array; older values are recomputed anyway */
    for (idx_t i = 0; i <= new_cap; ++i)
        tmp_tau[i] = 0, tmp_sum[i] = (seq_t)0;

    /* compute divisor counts via a sieve */
    for (idx_t d = 1; d <= new_cap; ++d) {
        for (idx_t m = d; m <= new_cap; m += d)
            ++tmp_tau[m];
    }

    /* compute prefix sums */
    tmp_sum[0] = (seq_t)0;
    for (idx_t i = 1; i <= new_cap; ++i)
        tmp_sum[i] = tmp_sum[i - 1] + (seq_t)tmp_tau[i];

    /* commit the new arrays */
    tau   = tmp_tau;
    sumTau= tmp_sum;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    ensure_capacity(n);

    seq_t res = (seq_t)1;
    res += (seq_t)4 * (seq_t)n;
    res += (seq_t)4 * sumTau[n];
    return res;
}

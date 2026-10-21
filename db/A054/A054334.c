/*
 * A054334  1/512 of 11th unsigned column of triangle A053120 (T-Chebyshev,
 *          rising powers, zeros omitted).
 * https://oeis.org/A054334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*binomial(n+10,10) - binomial(n+9,9)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 33, u64 n <= 351, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute binomial(n, k) for k <= 10 using 128‑bit arithmetic
 * and return the result as seq_t.  The function is safe for all
 * values of n for which the final result fits into seq_t. */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;

    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (res * (unsigned __int128)(n - k + i)) / i;
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    seq_t bin10 = binom_small(n + 10, 10);
    seq_t bin9  = binom_small(n + 9, 9);

    unsigned __int128 tmp = (unsigned __int128)bin10 * 2;
    tmp -= (unsigned __int128)bin9;
    return (seq_t)tmp;
}

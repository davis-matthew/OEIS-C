/*
 * A061128  Number of degree-n permutations of order exactly 30.
 * https://oeis.org/A061128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   * a(n) = Σ_{d|30} μ(30/d) * f_d(n) * * where f_d(n) counts
 *            permutations of n whose all cycle lengths divide d, * computed
 *            by the recurrence * f_d(0) = 1 * f_d(m) = Σ_{l|d, 1≤l≤m}
 *            (m-1)!/(m-l)! * f_d(m-l) * * This implementation uses the six
 *            divisors of 30 and * the Möbius values μ(30/d) = ±1 as g...
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   count_with_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/count_with_divisor.h"

/* Divisors of 30 */
static const idx_t divs[8] = { 1, 2, 3, 5, 6, 10, 15, 30 };

/* Corresponding Möbius μ(30/d) values: -1, +1, +1, +1, -1, -1, -1, +1 */
static const int mu[8] = { -1, 1, 1, 1, -1, -1, -1, 1 };

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* precompute factorials up to n */
    seq_t *fact = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!fact) return (seq_t)0;
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) fact[i] = fact[i - 1] * (seq_t)i;

    seq_t pos = (seq_t)0;    /* sum of positive Möbius terms */
    seq_t neg = (seq_t)0;    /* sum of negative Möbius terms */

    for (int i = 0; i < 8; ++i) {
        seq_t val = count_with_divisor(n, divs[i], fact);
        if (mu[i] > 0)
            pos += val;
        else
            neg += val;
    }

    free(fact);
    return pos - neg;
}

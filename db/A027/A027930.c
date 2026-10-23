/*
 * A027930  a(n) = T(n, 2*n-7), T given by A027926.
 * https://oeis.org/A027930
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \binom{n}{7} + \binom{n-1}{5} + \binom{n-2}{3} +
 *            \binom{n-3}{1} (implements the defining sum with a small
 *            multiplicative binomial helper)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 82, u64 n <= 1003,
 *            u128 n <= 1003, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* small multiplicative binomial: works for k <= 7, safe for any idx_t n */
static seq_t binomial_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;          /* use the smaller argument */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res = res * (n - k + i) / i;  exact division */
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;               /* undefined below the offset */
    return binomial_small(n,   7) +
           binomial_small(n-1, 5) +
           binomial_small(n-2, 3) +
           binomial_small(n-3, 1);
}

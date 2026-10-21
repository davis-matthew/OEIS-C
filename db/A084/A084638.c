/*
 * A084638  Binomial transform of (1,0,1,0,1,0,1,0,2,0,2,0,2,....).
 * https://oeis.org/A084638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * 2^(n-1) - (1 + C(n,2) + C(n,4) + C(n,6)) (n ≥ 0)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to the OEIS definition: a(n) = Σ_{k=0..3} C(n,2k) + 2*Σ_{k=4..⌊n/2⌋} C(n,2k) */

#include <stdbool.h>
#include "../utils/pow2.h"

/* Compute binomial coefficient C(n,k) for small k (k ≤ 6) */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        /* Multiply by (n - k + i) */
        res = res * (seq_t)(n - k + i);
        /* Divide by i (exact division) */
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum_even;
    if (n == (idx_t)0)
        sum_even = (seq_t)1;           /* C(0,0) = 1 */
    else
        sum_even = pow2(n - (idx_t)1); /* 2^(n‑1) for n ≥ 1 */

    seq_t ans = sum_even + sum_even;   /* 2 * 2^(n‑1) */

    if (n >= (idx_t)2)
        ans -= binom_small(n, (idx_t)2);
    if (n >= (idx_t)4)
        ans -= binom_small(n, (idx_t)4);
    if (n >= (idx_t)6)
        ans -= binom_small(n, (idx_t)6);

    ans -= (seq_t)1;                   /* subtract the leading 1 */

    return ans;
}

/*
 * A047190  Number of nonempty subsets of {1,2,...,n} in which exactly 4/5 of
 *          the elements are <= (n-2)/2.
 * https://oeis.org/A047190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{t=1}^{min(⌊L/4⌋, H)} C(L,4t)·C(H,t) where L =
 *            ⌊(n‑2)/2⌋, H = n – L
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* In other words, count the nonempty subsets of {1,…,n} that contain
   exactly 4/5 of their elements from ≤(n‑2)/2. */
#include <stdint.h>

static seq_t binom_unsigned(idx_t n, idx_t k)
{
    /* Compute the binomial coefficient C(n,k) using a multiplicative
       algorithm that does not overflow posibly in the intermediate
       steps of arbitrary‑precision seq_t. */
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* L = floor((n-2)/2)  =>  n/2 - 1 for unsigned n >= 2 */
    idx_t L = n / 2 - 1;
    /* H = n - L  (elements > L) */
    idx_t H = n - L;

    idx_t t_max1 = L / 4;   /* max t from low‑side constraint */
    idx_t t_max2 = H;       /* max t from high‑side constraint */
    idx_t t_max = t_max1 < t_max2 ? t_max1 : t_max2;

    seq_t sum = 0;
    for (idx_t t = 1; t <= t_max; ++t) {
        seq_t low  = binom_unsigned(L, 4 * t);
        seq_t high = binom_unsigned(H, t);
        sum += low * high;
    }
    return sum;
}

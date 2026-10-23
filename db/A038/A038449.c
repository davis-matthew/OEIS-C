/*
 * A038449  Sums of 7 distinct powers of 10.
 * https://oeis.org/A038449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th smallest integer having exactly seven decimal
 *            digits equal to 1 and all other digits equal to 0. The sequence
 *            is sorted by numeric value
 * Range:     u8 none, u16 none, u32 n <= 120, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

/* Compute binomial coefficient C(n,k) for 0 <= k <= n.
   The   res * (n - k + i) / i   division is exact, so no rounding
   error occurs. */
static inline idx_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the largest exponent k such that
       the number of terms with maximum exponent <= k
       is at least n.  The cumulative count is
       sum_{t=6}^{k} C(t,6) = C(k+1,7).  */
    idx_t k = 6;
    idx_t cumulative = 0;
    idx_t bin;
    while (1) {
        bin = binom(k, 6);
        if (cumulative + bin >= n) break;
        cumulative += bin;
        ++k;
    }

    /* Zero‑based rank r of the desired combination among all
       6‑subsets of {0,1,…,k-1}. */
    idx_t r = n - cumulative - 1;

    /* Unranking: reconstruct the 6 positions of the lower exponents.
       bits[i]==1 means the exponent i is present. */
    unsigned char bits[64] = {0};          /* k will never exceed 64 for practical n */
    idx_t ones_left = 6;
    for (idx_t pos = k; pos > 0; ) {      /* pos goes from k‑1 down to 0 */
        --pos;
        if (ones_left == 0) {             /* no more 1s to place */
            bits[pos] = 0;
            continue;
        }
        if (ones_left == pos + 1) {       /* must put 1 on all remaining slots */
            bits[pos] = 1;
            --ones_left;
            continue;
        }
        idx_t count0 = binom(pos, ones_left);  /* ways if current bit is 0 */
        if (r < count0) {
            bits[pos] = 0;
        } else {
            bits[pos] = 1;
            r -= count0;
            --ones_left;
        }
    }

    /* Compute the numeric value: 10^k + Σ_{i where bits[i]==1} 10^i. */
    seq_t sum = pow10(k);
    for (idx_t i = 0; i < k; ++i) {
        if (bits[i]) sum += pow10(i);
    }
    return sum;
}

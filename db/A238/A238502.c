/*
 * A238502  Let k = A239161(n); then a(n) = (sum of divisors of k) /(sum of
 *          divisors d of k with d <= sqrt(k)).
 * https://oeis.org/A238502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(k) / sigma_small(k) where k = the n-th integer such
 *            that sigma(k)/sigma_small(k) is an integer (i.e., k is the n-th
 *            member of OEIS A239161). sigma(k) is the sum of all positive
 *            divisors of k. sigma_small(k) is the sum of the divisors of k
 *            that are <= sqrt(k)
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, rec_sigma_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/rec_sigma_small.h"

/* integer square‑root for 64‑bit values */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid != 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            idx_t sq = mid * mid;
            if (sq == n) return mid;
            if (sq < n)  low = mid + 1;
            else        high = mid - 1;
        }
    }
    return high;
}

/* main sequence generator */
static seq_t *ratios   = NULL;
static size_t  ratios_cap = 0;
static size_t  ratios_len = 0;
static idx_t   next_k    = 1;

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    /* extend precomputation until we have at least the n-th term */
    while (ratios_len < (size_t)n) {
        uint64_t k = next_k++;
        /* factor k */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_u64(k, primes, exps, &cnt);

        /* compute sigma(k) */
        seq_t sigma = 1;
        for (size_t i = 0; i < cnt; ++i) {
            seq_t pe = 1;
            for (uint64_t e = 0; e < exps[i]; ++e) {
                pe *= (seq_t)primes[i];
            }
            seq_t term = (pe * (seq_t)primes[i] - 1) / ((seq_t)primes[i] - 1);
            sigma *= term;
        }

        /* compute sigma_small(k) */
        seq_t sigma_small = 0;
        idx_t sqrt_k = isqrt_u64(k);
        rec_sigma_small(0, (seq_t)1, primes, exps, cnt, sqrt_k, &sigma_small);

        if (sigma_small == 0) continue;                     /* avoid division by zero */
        if (sigma % sigma_small == 0) {                     /* integer ratio found */
            seq_t ratio = sigma / sigma_small;

            /* store the ratio */
            if (ratios_len >= ratios_cap) {
                size_t new_cap = (ratios_cap == 0) ? 8 : ratios_cap * 2;
                seq_t *tmp = (seq_t *)realloc(ratios, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                ratios   = tmp;
                ratios_cap = new_cap;
            }
            ratios[ratios_len++] = ratio;
        }
    }
    return ratios[n - 1];
}

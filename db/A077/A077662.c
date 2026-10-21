/*
 * A077662  a(n) = n-th positive integer not relatively prime to n, with
 *          a(1)=1.
 * https://oeis.org/A077662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer not relatively prime with n (i.e.
 *            gcd(m,n) > 1). The sequence starts a(1)=1. For each n the
 *            smallest m such that exactly n integers <= m are not coprime to
 *            n is returned
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_primes.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */

    /* find distinct prime factors of n */
    uint64_t primes[64];
    int k;
    factor_primes(n, primes, &k);

    /* largest prime factor – used for the upper bound */
    uint64_t maxp = primes[0];
    for (int i = 1; i < k; ++i)
        if (primes[i] > maxp) maxp = primes[i];

    /* binary search for the minimal m with count_not_coprime(m) >= n */
    seq_t low  = (seq_t)1;
    seq_t high = (seq_t)n * (seq_t)maxp;     /* safe for seq_t large enough */

    while (low < high) {
        seq_t mid = low + (high - low) / 2;

        /* inclusion–exclusion count of integers <= mid
           that share a prime factor with n */
        seq_t tot = 0;
        int subset_cnt = 1 << k;            /* k <= 15 for 64‑bit n */

        for (int mask = 1; mask < subset_cnt; ++mask) {
            seq_t pprod = 1;
            int bits = 0;
            for (int i = 0; i < k; ++i) {
                if (mask & (1 << i)) {
                    pprod *= (seq_t)primes[i];
                    ++bits;
                }
            }
            seq_t term = mid / pprod;
            if (bits & 1) tot += term;
            else          tot -= term;
        }

        if (tot >= (seq_t)n) high = mid;
        else                 low  = mid + 1;
    }
    return low;
}

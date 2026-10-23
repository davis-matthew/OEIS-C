/*
 * A174939  a(n) = Sum_{k<=n} A007955(k) * A007955(k) = Sum_{k<=n}
 *          A007955(k)^2, where A007955(m) = product of divisors of m.
 * https://oeis.org/A174939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} k^{d(k)} , where d(k) is the number of
 *            divisors of k
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 59, u128 n <= 179,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Linear sieve for smallest prime factor (spf). */
    uint64_t *spf = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (!spf)
        abort();
    for (idx_t i = 0; i <= n; ++i)
        spf[i] = 0;

    uint64_t *prime = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (!prime) {
        free(spf);
        abort();
    }
    idx_t pcnt = 0;

    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            prime[pcnt++] = i;
        }
        for (idx_t j = 0; j < pcnt && prime[j] <= spf[i] && i * prime[j] <= n; ++j)
            spf[i * prime[j]] = prime[j];
    }

    /* Accumulate the sum Σ k^{d(k)}. */
    seq_t total = 0;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t temp = k;
        idx_t d = 1;                 /* divisor count d(k) */

        /* Factor k using the spf table and compute d(k). */
        while (temp > 1) {
            uint64_t p = spf[temp];
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            d *= (e + 1);
        }

        /* Add k^d to the total. */
        seq_t power = big_pow((seq_t)k, (seq_t)d);
        total += power;
    }

    free(spf);
    free(prime);
    return total;
}

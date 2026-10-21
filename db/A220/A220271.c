/*
 * A220271  Product of the radicals of the integers less than n and relatively
 *          prime to n.
 * https://oeis.org/A220271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{1≤k<n, gcd(k,n)=1} rad(k) where rad(k)=∏_{p|k} p
 *            (product of distinct prime factors of k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   build_spf, factor_u64, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_spf.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }

    /* -- Cache for radicals up to the largest n seen so far -- */
    static idx_t rad_limit = 0;
    static seq_t *rad_cache = NULL;
    static idx_t *spf_cache = NULL;   /* smallest prime factor array */

    idx_t needed = n - 1;
    if (needed > rad_limit) {
        /* free old cache */
        if (rad_cache) free(rad_cache);
        if (spf_cache) free(spf_cache);

        /* build spf array up to needed */
        build_spf(needed, &spf_cache);

        /* allocate and compute radical values */
        rad_cache = (seq_t *)calloc((size_t)(needed + 1), sizeof(seq_t));
        rad_cache[0] = (seq_t)0;
        rad_cache[1] = (seq_t)1;
        for (idx_t i = 2; i <= needed; ++i) {
            idx_t p = spf_cache[i];
            idx_t temp = i;
            while (temp % p == 0) temp /= p;
            rad_cache[i] = seq_mul(rad_cache[temp], (seq_t)p);
        }
        rad_limit = needed;
    }

    /* Factor n to obtain its distinct prime divisors */
    size_t fcnt = 0;
    uint64_t *primes = (uint64_t *)calloc(64, sizeof(uint64_t));
    uint64_t *exps    = (uint64_t *)calloc(64, sizeof(uint64_t));
    factor_u64((uint64_t)n, primes, exps, &fcnt);

    seq_t result = (seq_t)1;
    for (idx_t k = 1; k < n; ++k) {
        bool coprime = true;
        for (size_t i = 0; i < fcnt; ++i) {
            if (k % (idx_t)primes[i] == 0) {
                coprime = false;
                break;
            }
        }
        if (coprime) {
            result = seq_mul(result, rad_cache[k]);
        }
    }

    free(primes);
    free(exps);
    return result;
}

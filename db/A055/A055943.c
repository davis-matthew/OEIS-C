/*
 * A055943  Smallest k >1 such that k | Sigma(k) - n.
 * https://oeis.org/A055943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k > 1 such that k | σ(k) - n * where σ(k) is the
 *            sum of all positive divisors of k
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   sigma_naive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_naive.h"

#define LIMIT_K 200000

static bool initialized = false;
static uint32_t spf[LIMIT_K + 1];
static uint64_t sigma_cache[LIMIT_K + 1];

static void init(void) {
    /* compute smallest prime factor for each number up to LIMIT_K */
    for (uint32_t i = 2; i <= LIMIT_K; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((uint64_t)i * i <= LIMIT_K) {
                for (uint32_t j = i * i; j <= LIMIT_K; j += i) {
                    if (spf[j] == 0)
                        spf[j] = i;
                }
            }
        }
    }
    /* sigma_cache is already zero-initialised (static storage) */
    initialized = true;
}

/* compute σ(n) using precomputed spf (n <= LIMIT_K) */
static uint64_t sigma_fast(uint32_t n) {
    uint64_t result = 1;
    uint32_t x = n;
    while (x > 1) {
        uint32_t p = spf[x];
        uint32_t cnt = 0;
        while (x % p == 0) {
            x /= p;
            cnt++;
        }
        /* factor = 1 + p + p^2 + ... + p^cnt */
        uint64_t term = 1;
        uint64_t factor = 1;
        for (uint32_t i = 0; i < cnt; ++i) {
            term *= p;
            factor += term;
        }
        result *= factor;
    }
    return result;
}

seq_t a(idx_t n) {
    if (!initialized) init();
    uint64_t nn = (uint64_t)n;
    for (uint32_t k = 2; ; ++k) {
        uint64_t sigma_k;
        if (k <= LIMIT_K) {
            if (sigma_cache[k] == 0) {
                sigma_cache[k] = sigma_fast(k);
            }
            sigma_k = sigma_cache[k];
        } else {
            sigma_k = sigma_naive(k);
        }
        if ((sigma_k % k) == (nn % k)) {
            return (seq_t)k;
        }
    }
}

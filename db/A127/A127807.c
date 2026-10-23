/*
 * A127807  Least positive primitive root of (n-th prime)^2.
 * https://oeis.org/A127807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least positive primitive root of (prime(n))^2 * * Uses
 *            the following helper functions from the utils library: * -
 *            _prime_limit_for_idx * - build_primes_upto * - factor_u64 * -
 *            mod_pow
 * Range:     u8 n <= 49, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, factor_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility includes */
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

/* Static cache for the prime list */
static uint64_t *prime_array = NULL;
static size_t prime_cnt   = 0;
static size_t prime_alloc = 0;

/* Ensure that at least the first `need` primes are cached. */
static void cache_primes(idx_t need)
{
    if (prime_cnt >= (size_t)need)
        return;

    /* Estimate an upper bound that is guaranteed to contain `need` primes. */
    idx_t limit = _prime_limit_for_idx(need);
    /* Free any previously allocated array. */
    if (prime_array) {
        free(prime_array);
        prime_array = NULL;
    }

    /* Allocate a new array of primes up to `limit` and store the count. */
    build_primes_upto(limit, &prime_array, &prime_cnt);
}

/* Compute the least positive primitive root of p². */
static seq_t least_root_of_p2(uint64_t p)
{
    if (p == 2)
        return 3;                     /* Primitive root of 4 is 3 */

    seq_t p2 = (seq_t)p * (seq_t)p;     /* Modulus p² */

    /* Factor p‑1 --------------------------------------------------------- */
    uint64_t pf[64], ep[64];
    size_t cnt_pf = 0;
    factor_u64(p - 1, pf, ep, &cnt_pf);

    /* Search for the smallest primitive root modulo p² ------------------ */
    for (uint64_t g64 = 2; g64 < p; ++g64) {
        seq_t g = (seq_t)g64;

        /* 1. Check that g is a primitive root modulo p */
        bool ok = true;
        for (size_t i = 0; i < cnt_pf; ++i) {
            uint64_t q = pf[i];
            seq_t result = mod_pow(g, (p - 1) / q, (seq_t)p);
            if (result == 1) {
                ok = false;
                break;
            }
        }
        if (!ok)
            continue;

        /* 2. Check the necessary condition for p² */
        if (mod_pow(g, (p - 1), p2) != 1)
            return g;                 /* g is primitive modulo p² */
    }

    /* In theory this point is never reached. */
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* Undefined for n = 0 */

    cache_primes(n);
    uint64_t p = prime_array[n - 1];
    return least_root_of_p2(p);
}

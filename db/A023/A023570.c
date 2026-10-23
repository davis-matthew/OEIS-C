/*
 * A023570  Greatest exponent in prime-power factorization of prime(n)-3; a(1)
 *          = a(2) = 0.
 * https://oeis.org/A023570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the greatest exponent in the prime‑power factorization of prime(n)-3.
   a(1) = a(2) = 0.  For n>=3,
   a(n) = max_{p^e | (prime(n)-3)} e.
   Uses a static cache of primes and the provided factor_u64 helper. */

#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

static uint64_t *prime_cache = NULL;
static size_t   prime_cache_cnt = 0;
static size_t   prime_cache_limit = 0;
static int      prime_cache_initialized = 0;

/* Ensure that prime_cache contains at least n primes. */
static void
ensure_primes(idx_t n)
{
    if (!prime_cache_initialized || prime_cache_cnt < n) {
        /* Determine a limit that guarantees at least n primes. */
        idx_t limit = _prime_limit_for_idx(n);
        if (prime_cache_initialized && limit <= prime_cache_limit)
            limit = prime_cache_limit * 2 + 100;

        uint64_t *new_primes = NULL;
        size_t   new_cnt = 0;
        build_primes_upto(limit, &new_primes, &new_cnt);

        /* If the limit was too small, grow until we have enough. */
        while (new_cnt < (size_t)n) {
            limit *= 2;
            if (prime_cache) free(prime_cache);
            build_primes_upto(limit, &new_primes, &new_cnt);
        }

        if (prime_cache) free(prime_cache);
        prime_cache      = new_primes;
        prime_cache_cnt  = new_cnt;
        prime_cache_limit = limit;
        prime_cache_initialized = 1;
    }
}

seq_t
a(idx_t n)
{
    if (n < 3) return 0;                         /* a(1)=a(2)=0 */

    ensure_primes(n);

    /* prime(n) is stored at index n-1 (0‑based). */
    uint64_t p = prime_cache[n - 1];
    uint64_t val = p - 3;                       /* guaranteed >=2 for n>=3 */

    /* Factor val.  Use a stack‑based array; val is always small enough
       to fit into 64 bits. */
    uint64_t fac_pr[64];
    uint64_t fac_ex[64];
    size_t fac_cnt;

    factor_u64(val, fac_pr, fac_ex, &fac_cnt);

    /* Find maximum exponent. */
    seq_t max_exp = 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        if (fac_ex[i] > max_exp)
            max_exp = fac_ex[i];
    }

    return max_exp;
}

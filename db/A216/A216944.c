/*
 * A216944  The least odd prime p for which the n-th prime can be written as
 *          3p + q - 3 for some odd prime q.
 * https://oeis.org/A216944
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least odd prime p such that the n‑th prime can be
 *            written as 3p + q – 3 for some odd prime q. The sequence is
 *            indexed with offset 5 (a(5) is the first term)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

/* Cache of primes used to obtain the n‑th prime and to test q. */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;

/* Least odd prime p for which the n‑th prime can be expressed as
   3p + q – 3 with an odd prime q.  For n < 5 the value is undefined
   and the function returns 0. */
seq_t a(idx_t n)
{
    if (n < 5)
        return 0;

    /* Ensure we have at least n primes cached.  If the cached
       array is too small, rebuild it with a larger upper limit. */
    if (!prime_cache || prime_cache_cnt < (size_t)n) {
        /* Free the old cache before allocating a new one. */
        free(prime_cache);
        prime_cache = NULL;
        prime_cache_cnt = 0;

        idx_t limit = _prime_limit_for_idx(n);
        build_primes_upto(limit, &prime_cache, &prime_cache_cnt);
    }

    /* Retrieve the n‑th prime (0‑based index n-1). */
    uint64_t prime_n = prime_cache[(size_t)(n - 1)];

    /* Search odd primes p in ascending order until we find a suitable q.
       q = prime_n + 3 – 3p.  Since q decreases as p increases, we can
       stop when q ≤ 2, because q must be an odd prime (≥3). */
    for (size_t i = 1;  /* skip prime 2 */ i < prime_cache_cnt; ++i) {
        uint64_t p = prime_cache[i];               /* p is odd */
        int64_t  q_val = (int64_t)prime_n + 3LL
                        - 3LL * (int64_t)p;          /* may become negative */

        if (q_val <= 2)
            break;                                 /* further p will only make q smaller */

        uint64_t q = (uint64_t)q_val;
        if ((q & 1ULL) == 0)                         /* q must be odd */
            continue;

        if (is_prime((idx_t)q))
            return (seq_t)p;                         /* found the least odd prime p */
    }

    /* Under the conjecture that every prime ≥ 11 can be written in the
       required form, the loop above will always terminate.  This return
       is only a safety fallback. */
    return 0;
}

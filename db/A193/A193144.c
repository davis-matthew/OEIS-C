/*
 * A193144  Primes of the form n^2 + n + 1, where n is semiprime.
 * https://oeis.org/A193144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form n^2 + n + 1 where n is semiprime
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 22, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   build_primes_upto, ensure_capacity, is_prime_big,
 *            semiprime_factor_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"
#include "../utils/semiprime_factor_count.h"

/* Return the n-th term of the sequence.  The function works on
   arbitrary calls in any order and caches previously computed
   terms. */
seq_t a(idx_t n)
{
    /* One-time setup: build a small list of primes sufficient for
       factorisation of all semiprimes up to the maximum n requested. */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static bool primes_ready = false;

    /* Cache of previously found terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Highest semiprime value that has already been examined. */
    static idx_t max_checked = 0;

    /* Build prime list once.  1 000 000 is plenty for n up to 10 000. */
    if (!primes_ready) {
        build_primes_upto(1000000ULL, &primes, &primes_cnt);
        primes_ready = true;
    }

    /* If the caller requests an index that is already cached, return it. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Otherwise keep searching for new terms until we have enough. */
    for (idx_t k = max_checked + 1; ; ++k) {
        /* Check whether k is a semiprime. */
        if (semiprime_factor_count(k, primes, primes_cnt) == 2) {
            /* Compute k^2 + k + 1 in seq_t. */
            seq_t val = (seq_t)k * (seq_t)k + k + 1;
            if (is_prime_big(val)) {
                /* Grow the cache if necessary. */
                if (cache_len + 1 > cache_cap)
                    ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
                cache[cache_len++] = val;

                /* We have now produced the n-th term; remember the current k
                   so that future calls do not re‑process it. */
                max_checked = k;
                return val;
            }
        }
        /* Record that we have examined k, even if it was not accepted. */
        max_checked = k;
    }
}

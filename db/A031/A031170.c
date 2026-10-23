/*
 * A031170  a(n) = prime(n+8) - prime(n).
 * https://oeis.org/A031170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+8) - prime(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) using a dynamic sieve of primes up to the required index. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive indices; the sequence is defined for n ≥ 1. */
    if (n == 0)
        return (seq_t)0;

    /* Static state for memoization */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t max_idx_needed = 0;

    /* The largest index we need to be able to answer. */
    idx_t need = n + 8;

    /* Rebuild the prime table if we haven't computed enough primes yet. */
    if (need > max_idx_needed) {
        /* Determine a safe upper bound for the nth prime. */
        idx_t limit = _prime_limit_for_idx(need);
        uint64_t *new_primes = NULL;
        size_t new_cnt = 0;
        build_primes_upto(limit, &new_primes, &new_cnt);

        /* Replace the old table safely. */
        free(primes);
        primes     = new_primes;
        primes_cnt = new_cnt;
        max_idx_needed = need;
    }

    /* Safety check – if for some reason the table is smaller than required,
       just return 0 (this should not happen with a correct limit). */
    if (primes_cnt < (size_t)need)
        return (seq_t)0;

    /* Indices in the primes array are 0‑based, but prime(1) = primes[0]. */
    uint64_t p_hi = primes[need - 1];   /* prime(n+8) */
    uint64_t p_lo = primes[n - 1];      /* prime(n)   */

    return (seq_t)(p_hi - p_lo);
}

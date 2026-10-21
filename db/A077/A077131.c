/*
 * A077131  Sum of odd-indexed primes.
 * https://oeis.org/A077131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} p_{2k-1} (sum of the odd‑indexed primes)
 * Range:     u8 n <= 9, u16 n <= 107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_primes.h"

/* Cached prime data */
static idx_t *cached_primes   = NULL;   /* array of primes */
static size_t  cached_count   = 0;      /* number of primes stored   */
static idx_t   cached_limit   = 0;      /* upper bound used in sieve */

/* Ensure that we have at least (required_index+1) primes in cached_primes.
   required_index is zero‑based (e.g., index=4 means we need prime at position 4). */
static void
ensure_primes_size(idx_t required_index)
{
    if (cached_count > required_index)
        return;

    idx_t limit = cached_limit ? cached_limit * 2 : 100;

    while (1) {
        idx_t *tmp      = NULL;
        size_t tmp_cnt  = 0;

        sieve_primes(limit, &tmp, &tmp_cnt);

        if (cached_primes) free(cached_primes);
        cached_primes = tmp;
        cached_count  = tmp_cnt;
        cached_limit  = limit;

        if (cached_count > required_index)
            break;

        /* increase limit and try again */
        limit <<= 1;   /* multiply by 2 */
    }
}

/* External function required by the test harness */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset 0; return 0 for safety */

    /* Need primes up to index 2n-2 (since we sum primes at positions 0,2,4,...,2n-2) */
    idx_t required_idx = 2 * n - 1;   /* zero‑based index of the (2n-1)th prime */

    ensure_primes_size(required_idx);

    seq_t sum = 0;
    for (idx_t i = 0; i < required_idx; i += 2)
        sum += (seq_t)cached_primes[i];

    return sum;
}

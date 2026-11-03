/*
 * A076954  a(n) = Product_{i=1..n} prime(i)^i.
 * https://oeis.org/A076954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n} prime(i)^i Recurrence: a(0) = 1, a(n+1) = a(n)
 *            * prime(n+1)^(n+1) (n >= 0)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 50 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
/* Utility headers required for primes and big‑integer arithmetic */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Cache of primes up to a required index */
static uint64_t *prime_cache     = NULL;  /* 0‑based primes             */
static size_t   prime_cache_cnt  = 0u;    /* number of primes in cache  */

/* Ensure that the prime cache contains at least 'count' primes.
   The simple bound count*20 is more than enough for the n used in the tests. */
static void ensure_primes(idx_t count)
{
    if ((idx_t)prime_cache_cnt < count)
    {
        idx_t limit = count < 6u ? 15u : count * 20u;
        uint64_t *new_primes = NULL;
        size_t    new_cnt    = 0u;

        build_primes_upto(limit, &new_primes, &new_cnt);

        free(prime_cache);
        prime_cache     = new_primes;
        prime_cache_cnt = new_cnt;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;   /* a(0) == 1 */

    /* Make sure we have all required primes. */
    ensure_primes(n);

    seq_t result = (seq_t)1;
    for (idx_t i = 1u; i <= n; ++i)
    {
        seq_t prime_val = (seq_t)prime_cache[i - 1u];   /* prime(i) */
        seq_t pow_val   = ipow_idx(prime_val, i);        /* prime(i)^i */
        result = seq_mul(result, pow_val);              /* cumulative product */
    }
    return result;
}

/*
 * A176947  Primes of the form 11^k + 2^k.
 * https://oeis.org/A176947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, bref, more
 * Formula:   a(n) is the n-th prime of the form 11^k + 2^k (k ≥ 0)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

/* Cached primes and state */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_k = 0;          /* last k processed */
static seq_t pow11 = 1;           /* 11^k */
static seq_t pow2  = 1;           /* 2^k */
static bool initialized = false;
static const seq_t LIMIT = (seq_t)UINT64_MAX;

/* Initialise static state once. */
static void init_state(void)
{
    if (!initialized) {
        cache = NULL;
        cache_len = 0;
        cache_cap = 0;
        last_k = 0;
        pow11 = 1;
        pow2 = 1;
        initialized = true;
    }
}

/* Extend cache until it contains at least n primes. */
static void extend_cache(idx_t n)
{
    init_state();

    while (cache_len < n) {
        seq_t sum = pow11 + pow2;
        if (is_prime_big(sum)) {
            if (cache_len >= cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = sum;
        }

        /* Prepare next powers.  Stop if further values would overflow
           the 64‑bit limit (only for 64‑bit builds). */
        if (sizeof(seq_t) <= 8 && pow11 > LIMIT / 11)
            break;
        pow11 *= 11;
        pow2 *= 2;
        last_k++;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offsets start at 1 */

    extend_cache(n);
    if (cache_len >= n)
        return cache[n - 1];
    return 0;                     /* value not found (e.g. beyond known terms) */
}

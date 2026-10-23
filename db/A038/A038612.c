/*
 * A038612  Primes not containing the digit '4'.
 * https://oeis.org/A038612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 4101, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_four, is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime that does not contain the digit '4' by incremental search. */
/* No closed formula is known for this sequence. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"
#include "../utils/has_four.h"

/* Cache for already computed terms. */
static seq_t *cache = NULL;
static size_t cap   = 0;
static size_t len   = 0;

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache(size_t needed)
{
    if (needed <= cap)
        return;
    size_t new_cap = needed;
    cache = seq_realloc(cache, cap, new_cap);
    cap   = new_cap;
}

/* Main function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0u)               /* offset starts at 1; undefined for 0 */
        return 0;

    size_t need = (size_t)n;
    ensure_cache(need);

    while (len < need) {
        /* Start candidate at 2 if none found yet, otherwise next integer. */
        idx_t cand = (len == 0) ? 2u : (idx_t)(cache[len - 1]) + 1u;
        for (;;) {
            if (!has_four(cand) && is_prime(cand)) {
                cache[len] = (seq_t)cand;
                ++len;
                break;
            }
            ++cand;
        }
    }
    return cache[need - 1];
}

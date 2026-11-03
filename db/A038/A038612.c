/*
 * A038612  Primes not containing the digit '4'.
 * https://oeis.org/A038612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 4101, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9779 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime that does not contain the digit '4' by incremental search. */
/* No closed formula is known for this sequence. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Returns true if v contains the decimal digit 4. */
static bool has_four(idx_t v)
{
    while (v > 0u) {
        if (v % 10u == 4u)
            return true;
        v /= 10u;
    }
    return false;
}

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

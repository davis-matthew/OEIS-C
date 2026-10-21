/*
 * A074772  Numbers k such that tau(k) < tau(k+1) and phi(k) < phi(k+1).
 * https://oeis.org/A074772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that τ(k) < τ(k+1) and φ(k) <
 *            φ(k+1). This implementation performs a linear scan of the
 *            integers and memoises the found values in a growable array so
 *            that subsequent calls, in any order, return the correct entry
 * Range:     u8 n <= 7, u16 n <= 4337, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/divisor_count_u64.h"   /* divisor_count_u64(uint64_t) */
#include "../utils/calc_phi.h"            /* calc_phi(seq_t) */

/* Static cache for the sequence values. */
static seq_t *cache      = NULL;   /* array of found k's */
static size_t cache_len  = 0;      /* number of stored terms */
static size_t cache_cap  = 0;      /* allocated capacity */
static uint64_t last_k_checked = 1;/* next integer to examine */

/* Grow the cache array when it runs out of space. */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* Fatal error: out‑of‑memory. */
        exit(EXIT_FAILURE);
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Return the nth term of sequence A074772 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n < 1 – return 0 safely. */
        return (seq_t)0;
    }

    /* Extend the cache until it contains the nth term. */
    while (cache_len < (size_t)n) {
        /* Compute τ and φ for the current k and k+1. */
        seq_t tau_k  = (seq_t)divisor_count_u64(last_k_checked);
        seq_t tau_k1 = (seq_t)divisor_count_u64(last_k_checked + (uint64_t)1);

        seq_t phi_k  = calc_phi((seq_t)last_k_checked);
        seq_t phi_k1 = calc_phi((seq_t)(last_k_checked + (uint64_t)1));

        /* Record k if both inequalities hold. */
        if (tau_k < tau_k1 && phi_k < phi_k1) {
            if (cache_len >= cache_cap) {
                grow_cache();
            }
            cache[cache_len++] = (seq_t)last_k_checked;
        }

        ++last_k_checked;
    }

    return cache[(size_t)n - 1];
}

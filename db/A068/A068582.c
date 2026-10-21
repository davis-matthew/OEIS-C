/*
 * A068582  Let phi_m(x) = phi(phi(...(phi(x))...)) m times; sequence gives
 *          values of k such that phi_5(k) = tau(k).
 * https://oeis.org/A068582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 340, u32 n <= 340, u64 n <= 340,
 *            u128 n <= 340, big any n
 * Helpers:   calc_phi, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A068582 – find k such that φ⁵(k) = τ(k) (φ iterated 5 times equals the divisor‑count). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/calc_phi.h"
#include "../utils/divisor_count_u64.h"

/* Helper: compute φ⁵(x) for a sequence value x. */
static seq_t phi5(seq_t x)
{
    seq_t t = calc_phi(x);
    t = calc_phi(t);
    t = calc_phi(t);
    t = calc_phi(t);
    t = calc_phi(t);
    return t;
}

/* Static cache for found terms of the sequence. */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;

/* Ensure the cache has enough capacity. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* out of range, safety only */

    /* If we already know enough terms, return immediately. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Build terms until we have at least n of them. */
    while (cache_len < (size_t)n)
    {
        uint64_t k = next_candidate++;

        seq_t phi5k = phi5((seq_t)k);
        seq_t tau    = (seq_t)divisor_count_u64(k);

        if (phi5k == tau)
        {
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = (seq_t)k;
        }
    }
    return cache[n - 1];
}

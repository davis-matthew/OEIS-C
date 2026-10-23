/*
 * A045560  Numbers whose factorial has '48' as its final two digits before
 *          the trailing zeros.
 * https://oeis.org/A045560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 3299, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_mod2, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose factorial has “48” as its final two digits before the trailing zeros. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"
#include "../utils/pow_mod2.h"

static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t *cache = NULL;
static idx_t next_k = 1;          /* Next integer to test */
static seq_t res_mod100 = 1;      /* Product of all factors with 2s and 5s removed, modulo 100 */
static int diff = 0;              /* Number of 2s removed minus number of 5s removed */

static void try_add_current(idx_t k) {
    /* Compute final non‑zero two digits of k!  */
    seq_t tmp = res_mod100;
    tmp = (tmp * pow_mod2(diff)) % 100;
    if (tmp == 48) {
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            seq_t *tmp_cache = seq_realloc(cache, cache_cap, new_cap);
            cache = tmp_cache;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)k;
    }
}

seq_t a(idx_t n) {
    /* Grow cache until we have the n‑th term. */
    while (cache_len < n) {
        /* Process current_k */
        if (next_k > 1) {
            idx_t x = next_k;
            /* Remove all 2s and 5s from x, updating diff. */
            while ((x & 1) == 0) {  /* x % 2 == 0 */
                x >>= 1;
                diff++;
            }
            while (x % 5 == 0) {
                x /= 5;
                diff--;
            }
            res_mod100 = (res_mod100 * (seq_t)(x % 100)) % 100;
        }
        try_add_current(next_k);
        next_k++;
    }
    return cache[n - 1];
}

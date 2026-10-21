/*
 * A067575  Numbers k that divide phi(k)*bigomega(k).
 * https://oeis.org/A067575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   bigomega, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th integer k ≥ 1 such that k | φ(k)·Ω(k),
 * where φ is Euler's totient and Ω counts prime factors with multiplicity. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/bigomega.h"

static seq_t *cache = NULL;   /* cached terms of the sequence */
static size_t cache_cap = 0;  /* allocation size of 'cache' */
static size_t cache_len = 0;  /* number of values currently cached */
static uint64_t last_k = 0;   /* largest integer examined so far */

seq_t a(idx_t n)
{
    /* 'n' is guaranteed to be ≥ 1 by the caller. */
    size_t idx = (size_t)n;            /* 1‑based index to look for */

    /* Ensure cache can hold at least 'idx' terms. */
    if (idx > cache_cap) {
        size_t new_cap = idx > 128 ? idx : 128;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new qualifying k until we have enough terms. */
    uint64_t k = last_k + 1;
    while (cache_len < idx) {
        seq_t phi_val = phi(k);          /* φ(k)   */
        seq_t omega_val = bigomega(k);   /* Ω(k)   */

        /* Convert k to the generic integer type. */
        seq_t kk = (seq_t)k;

        /* Check k | φ(k)*Ω(k). */
        if ((phi_val * omega_val) % kk == 0) {
            cache[cache_len++] = kk;
        }
        ++k;
    }
    last_k = k - 1;

    return cache[idx - 1];
}

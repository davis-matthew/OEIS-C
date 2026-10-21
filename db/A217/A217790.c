/*
 * A217790  Composite evil numbers (A001969) having no proper evil divisors.
 * https://oeis.org/A217790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1586, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitcount_seq_t, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A217790: Composite evil numbers having no proper evil divisors.
   Computed by iterating numbers and selecting those that
   - are composite,
   - are evil (even number of 1‑bits in binary),
   - have no proper divisor that is also evil. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/bitcount_seq_t.h"

static seq_t *seq_cache      = NULL;   /* array of sequence values      */
static size_t seq_cache_cap  = 0;       /* allocated capacity             */
static size_t seq_cache_len  = 0;       /* number of values stored        */
static idx_t  last_checked   = 3;       /* last number examined (start > 3) */

/* Return true if x has an even number of 1‑bits. */
static inline bool is_evil(idx_t x) {
    return ((bitcount_seq_t((seq_t)x) & 1) == 0);
}

/* Ensure seq_cache has space for at least 'need' elements. */
static void ensure_seq_capacity(size_t need) {
    if (seq_cache_cap >= need) return;
    size_t new_cap = seq_cache_cap ? (seq_cache_cap << 1) : 64;
    if (new_cap < need) new_cap = need;
    seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* Allocation failed – abort */
        exit(EXIT_FAILURE);
    }
    seq_cache = new_ptr;
    seq_cache_cap = new_cap;
}

/* Return the nth term of A217790 (1‑based). */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, guard against accidental 0 */

    /* Generate values until we have at least n of them. */
    while (seq_cache_len < (size_t)n) {
        idx_t m = last_checked + 1;
        last_checked = m;

        /* Skip primes. */
        if (is_prime_small((seq_t)m)) continue;

        /* Consider only evil composites. */
        if (!is_evil(m)) continue;

        /* Check that no proper divisor of m is evil. */
        bool ok = true;
        for (idx_t d = 2; d * d <= m; ++d) {
            if (m % d == 0) {
                idx_t e = m / d;
                if (is_evil(d)) { ok = false; break; }
                if (e != d && is_evil(e)) { ok = false; break; }
            }
        }
        if (!ok) continue;

        /* Store this valid number. */
        ensure_seq_capacity(seq_cache_len + 1);
        seq_cache[seq_cache_len++] = (seq_t)m;
    }
    return seq_cache[n - 1];
}

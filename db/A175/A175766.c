/*
 * A175766  Greater of "twin numbers": Omega(n) = Omega(n - 2^Omega(n)), where
 *          Omega is A001222.
 * https://oeis.org/A175766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = {x : x - 2^Ω(x) = y and Ω(x) = Ω(y)}, enumerated in
 *            increasing order. Here Ω(n) is the big-omega function from
 *            A001222
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   bigomega, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/bigomega.h"
#include "../utils/pow2.h"

static seq_t *cache = NULL;          /* cache of computed terms */
static size_t cache_cap = 0;         /* capacity of cache */
static idx_t computed = 0;           /* number of terms already cached */
static idx_t last_checked = 2;       /* last integer we examined for twin property */

/*
 * Helper: ensure cache can hold at least n terms.
 */
static void ensure_cache_capacity(idx_t n) {
    if (n <= cache_cap) return;
    size_t new_cap = (size_t)(n * 1.5 + 64);
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* If realloc fails, we let crash happen (as per usual contest code). */
    cache = new_ptr;
    cache_cap = new_cap;
}

/*
 * Main function requested by the problem statement.
 * Compute the nth greater twin number (1‑based indexing).
 */
seq_t a(idx_t n) {
    if (n < 1) return 0;  /* undefined for n=0 or negative, but return 0 */

    ensure_cache_capacity(n);

    /* Generate terms until we have at least n of them. */
    while (computed < n) {
        ++last_checked;                    /* candidate number */
        seq_t candidate = (seq_t)last_checked;

        /* Ω(candidate) */
        seq_t omega = bigomega(last_checked);

        /* Compute 2^Ω(candidate)  */
        seq_t pow2_omega = pow2((idx_t)omega);

        seq_t lower = candidate - pow2_omega;
        if (lower <= 0) continue;          /* skip if the difference is not positive */

        /* Ω(lower) */
        seq_t omega_lower = bigomega((idx_t)lower);

        if (omega == omega_lower) {        /* found a twin greater number */
            cache[computed++] = candidate;
        }
    }

    return cache[n - 1];
}
